/**
 * Pinpad / decryption logic for encrypted solution pages.
 *
 * Initialisation strategy (belt-and-suspenders):
 *  1. Run immediately if the DOM is already parsed (hard load / back-forward).
 *  2. Listen for DOMContentLoaded as a fallback for cold hard loads.
 *  3. Subscribe to Material's document$ observable to re-init on every
 *     instant-navigation page swap.
 *
 * A data attribute on the container prevents double-initialisation when
 * multiple paths happen to fire at once.
 */
function initPinpad() {
    const container = document.getElementById("encrypted-content");
    if (!container) return;                   // not an encrypted page
    if (container.dataset.pinpadInit) return; // already wired up
    container.dataset.pinpadInit = "1";
    let passwordValue = "";
    // ── helpers ──────────────────────────────────────────────────────────────
    function setPasswordValue(v) {
        passwordValue = v;
        const display = document.getElementById("led-display");
        if (display) display.textContent = passwordValue;
    }
    function appendHexText(text) {
        const sanitized = Array.from(text)
            .filter(c => /[0-9a-f]/i.test(c))
            .map(c => c.toUpperCase())
            .join("");
        if (!sanitized) return false;
        setPasswordValue(passwordValue + sanitized);
        const status = document.getElementById("decrypt-status");
        if (status) status.hidden = true;
        return true;
    }
    async function decryptTemplate(password) {
        const b64 = document.getElementById("template").textContent.trim();
        const bytes = Uint8Array.from(atob(b64), c => c.charCodeAt(0));
        const salt       = bytes.slice(0, 16);
        const iv         = bytes.slice(16, 28);
        const ciphertext = bytes.slice(28);
        const passwordKey = await crypto.subtle.importKey(
            "raw", new TextEncoder().encode(password),
            "PBKDF2", false, ["deriveKey"]
        );
        const aesKey = await crypto.subtle.deriveKey(
            { name: "PBKDF2", salt, iterations: 1000000, hash: "SHA-256" },
            passwordKey,
            { name: "AES-GCM", length: 256 },
            false, ["decrypt"]
        );
        const plaintext = await crypto.subtle.decrypt(
            { name: "AES-GCM", iv }, aesKey, ciphertext
        );
        return new TextDecoder().decode(plaintext);
    }
    async function handleDecrypt() {
        const status = document.getElementById("decrypt-status");
        if (status) { status.hidden = false; status.textContent = "..."; }
        try {
            const decryptedHtml = await decryptTemplate(passwordValue.toLowerCase());
            const parsed = new DOMParser().parseFromString(decryptedHtml, "text/html");
            const replacement = parsed.body.firstElementChild;
            if (!replacement) throw new Error("Decrypted content is empty");
            cleanup();
            document.getElementById("encrypted-content").replaceWith(replacement);
        } catch (err) {
            if (status) status.textContent = "Error!";
            console.error("Decryption failed:", err);
            setPasswordValue("");
        }
    }
    // ── keyboard / paste ─────────────────────────────────────────────────────
    function isEditable(target) {
        return target instanceof HTMLElement && (
            target.isContentEditable ||
            ["INPUT", "TEXTAREA", "SELECT"].includes(target.tagName)
        );
    }
    function onKeydown(event) {
        if (isEditable(event.target)) return;
        if (event.ctrlKey || event.metaKey) return;
        if (/^[0-9a-f]$/i.test(event.key)) {
            event.preventDefault(); event.stopPropagation();
            appendHexText(event.key);
        } else if (event.key === "Backspace") {
            event.preventDefault(); event.stopPropagation();
            setPasswordValue(passwordValue.slice(0, -1));
        } else if (event.key === "Enter") {
            event.preventDefault(); event.stopPropagation();
            handleDecrypt();
        }
    }
    async function onPaste(event) {
        if (isEditable(event.target)) return;
        let text = event.clipboardData?.getData("text") ?? "";
        if (!text) text = await navigator.clipboard?.readText?.().catch(() => "") ?? "";
        if (!/[0-9a-f]/i.test(text)) return;
        event.preventDefault(); event.stopPropagation();
        appendHexText(text);
    }
    function cleanup() {
        window.removeEventListener("keydown", onKeydown, true);
        window.removeEventListener("paste",   onPaste,   true);
    }
    // ── wire up DOM ──────────────────────────────────────────────────────────
    container.querySelectorAll(".hex-btn").forEach(btn =>
        btn.addEventListener("click", () => appendHexText(btn.dataset.key))
    );
    const backBtn    = document.getElementById("backspace-btn");
    const decryptBtn = document.getElementById("decrypt-button");
    if (backBtn)    backBtn.addEventListener("click",    () => setPasswordValue(passwordValue.slice(0, -1)));
    if (decryptBtn) decryptBtn.addEventListener("click", handleDecrypt);
    window.addEventListener("keydown", onKeydown, true);
    window.addEventListener("paste",   onPaste,   true);
    setPasswordValue("");
}
// ── Bootstrap ────────────────────────────────────────────────────────────────
// 1. Subscribe to Material's instant-navigation observable (fires on every
//    page swap, including the very first load because it's a ReplaySubject).
if (typeof document$ !== "undefined") {
    document$.subscribe(() => initPinpad());
}
// 2. Also hook DOMContentLoaded + immediate-ready as a safety net for hard
//    loads when document$ isn't yet defined or fires before we subscribe.
if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initPinpad);
} else {
    initPinpad();
}
