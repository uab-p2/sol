/**
 * Pinpad / decryption logic for encrypted solution pages.
 *
 * The LED display is a plain <input type="text"> — the user can type, paste,
 * and edit freely.  The ⌫ button removes the last character; the ▶ button
 * (or Enter) decrypts using the current value lowercased with spaces stripped.
 *
 * Bootstrap: hooks into Material's document$ observable for instant-navigation
 * re-init, and also runs on DOMContentLoaded / immediately for hard loads.
 * A data attribute guards against double-initialisation.
 */
function initPinpad() {
    const container = document.getElementById("encrypted-content");
    if (!container) return;
    if (container.dataset.pinpadInit) return;
    container.dataset.pinpadInit = "1";
    const templateEl = document.getElementById("template");
    const templateBlob = templateEl ? templateEl.textContent.trim() : "";
    const sessionKey = `pinpad:${location.pathname}:${templateBlob.length}:${templateBlob.slice(0, 48)}`;
    const input      = document.getElementById("led-display");
    const backBtn    = document.getElementById("backspace-btn");
    const decryptBtn = document.getElementById("decrypt-button");
    const status     = document.getElementById("decrypt-status");

    function jumpToHashIfPresent() {
        if (!location.hash) return;
        const id = decodeURIComponent(location.hash.slice(1));
        const target = document.getElementById(id);
        if (target) target.scrollIntoView();
    }

    function wireSamePageHashLinks(root) {
        const anchors = root.querySelectorAll("a[href]");
        anchors.forEach((anchor) => {
            anchor.addEventListener("click", (event) => {
                const href = anchor.getAttribute("href");
                if (!href) return;
                const url = new URL(href, location.href);
                if (url.origin !== location.origin || url.pathname !== location.pathname || !url.hash) return;
                event.preventDefault();
                history.replaceState(null, "", url.hash);
                jumpToHashIfPresent();
            });
        });
    }

    function restoreDecryptedContent() {
        try {
            const cachedHtml = sessionStorage.getItem(sessionKey);
            if (!cachedHtml) return false;
            const parsed = new DOMParser().parseFromString(cachedHtml, "text/html");
            const replacement = parsed.body.firstElementChild;
            if (!replacement) {
                sessionStorage.removeItem(sessionKey);
                return false;
            }
            container.replaceWith(replacement);
            wireSamePageHashLinks(replacement);
            jumpToHashIfPresent();
            return true;
        } catch (err) {
            console.warn("Unable to restore decrypted content:", err);
            return false;
        }
    }

    function cacheDecryptedContent(replacement) {
        try {
            sessionStorage.setItem(sessionKey, replacement.outerHTML);
        } catch (err) {
            console.warn("Unable to cache decrypted content:", err);
        }
    }

    if (restoreDecryptedContent()) return;
    if (!input || !decryptBtn) return;

    function clearStatus() {
        if (!status) return;
        status.textContent = "";
        status.hidden = true;
    }

    function showStatus(message) {
        if (!status) return;
        status.hidden = false;
        status.textContent = message;
    }
    // ── crypto ───────────────────────────────────────────────────────────────
    async function decrypt(password) {
        const b64 = templateBlob;
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
    // ── accept handler ───────────────────────────────────────────────────────
    async function handleAccept() {
        const password = input.value.toLowerCase().replace(/\s/g, "");
        if (!password) return;
        showStatus("Decrypting...");
        try {
            const decryptedHtml = await decrypt(password);
            const parsed = new DOMParser().parseFromString(decryptedHtml, "text/html");
            const replacement = parsed.body.firstElementChild;
            if (!replacement) throw new Error("Decrypted content is empty");
            document.getElementById("encrypted-content").replaceWith(replacement);
            wireSamePageHashLinks(replacement);
            cacheDecryptedContent(replacement);
            jumpToHashIfPresent();
        } catch (err) {
            showStatus("Error!");
            console.error("Decryption failed:", err);
            input.value = "";
            input.focus();
        }
    }
    // ── wire up ──────────────────────────────────────────────────────────────
    if (backBtn) {
        backBtn.addEventListener("click", () => {
            clearStatus();
            input.value = input.value.slice(0, -1);
            input.focus();
        });
    }
    decryptBtn.addEventListener("click", () => {
        clearStatus();
        handleAccept();
    });
    input.addEventListener("input", clearStatus);
    input.addEventListener("keydown", (e) => {
        if (e.key === "Enter") {
            e.preventDefault();
            clearStatus();
            handleAccept();
        }
    });
    clearStatus();
    input.focus();
}
// ── Bootstrap ────────────────────────────────────────────────────────────────
if (typeof document$ !== "undefined") {
    document$.subscribe(() => initPinpad());
}
if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initPinpad);
} else {
    initPinpad();
}
