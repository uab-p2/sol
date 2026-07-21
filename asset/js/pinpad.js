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
    if (container.dataset.pinpadInit === "1") return;
    const input      = container.querySelector("#led-display");
    const backBtn    = container.querySelector("#backspace-btn");
    const decryptBtn = container.querySelector("#decrypt-button");
    const status     = container.querySelector("#decrypt-status");

    function normalizePath(pathname) {
        const normalized = pathname.replace(/\/+$/, "");
        return normalized || "/";
    }

    function isSamePageHashUrl(url) {
        return url.origin === location.origin
            && normalizePath(url.pathname) === normalizePath(location.pathname)
            && !!url.hash;
    }

    function jumpToHashIfPresent() {
        if (!location.hash) return;
        const id = decodeURIComponent(location.hash.slice(1));
        const target = document.getElementById(id);
        if (target) target.scrollIntoView();
    }

    function installSamePageHashInterceptor() {
        if (window.__pinpadHashInterceptorInstalled) return;
        window.__pinpadHashInterceptorInstalled = true;

        document.addEventListener("click", (event) => {
            const anchor = event.target.closest("a[href]");
            if (!anchor) return;
            if (!document.querySelector("[data-pinpad-decrypted='1']")) return;

            const href = anchor.getAttribute("href");
            if (!href) return;

            let url;
            try {
                url = new URL(href, location.href);
            } catch {
                return;
            }

            if (!isSamePageHashUrl(url)) return;
            event.preventDefault();
            history.replaceState(null, "", url.hash);
            jumpToHashIfPresent();
        }, true);
    }

    installSamePageHashInterceptor();

    if (!input || !decryptBtn) return;

    function getTemplateBlob() {
        const templateEl = container.querySelector("#template");
        return templateEl ? templateEl.textContent.trim() : "";
    }

    const templateBlob = getTemplateBlob();
    if (!templateBlob) {
        if (!container.dataset.pinpadRetryScheduled) {
            container.dataset.pinpadRetryScheduled = "1";
            if (status) {
                status.hidden = false;
                status.textContent = "Debug: encrypted payload not ready yet, retrying...";
            }
            console.warn("Pinpad init: encrypted payload missing, retrying shortly.");
            setTimeout(() => {
                container.dataset.pinpadRetryScheduled = "";
                initPinpad();
            }, 80);
        }
        return;
    }

    const passwordCacheKey = `pinpad-password:${normalizePath(location.pathname)}:${templateBlob.length}:${templateBlob.slice(0, 48)}`;

    function getCachedPassword() {
        try {
            return sessionStorage.getItem(passwordCacheKey) || "";
        } catch {
            return "";
        }
    }

    function setCachedPassword(password) {
        try {
            sessionStorage.setItem(passwordCacheKey, password);
        } catch {
            // Ignore storage failures (private mode / quota / policies)
        }
    }

    function clearCachedPassword() {
        try {
            sessionStorage.removeItem(passwordCacheKey);
        } catch {
            // Ignore storage failures (private mode / quota / policies)
        }
    }

    container.dataset.pinpadInit = "1";

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
        const b64 = getTemplateBlob();
        if (!b64) throw new Error("Encrypted payload is missing");
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
    async function handleAccept(passwordOverride = null, options = {}) {
        const password = (passwordOverride === null ? input.value : passwordOverride)
            .toLowerCase()
            .replace(/\s/g, "");
        if (!password) return;
        if (!options.silent) showStatus("Decrypting...");
        try {
            const decryptedHtml = await decrypt(password);
            const parsed = new DOMParser().parseFromString(decryptedHtml, "text/html");
            const replacement = parsed.body.firstElementChild;
            if (!replacement) throw new Error("Decrypted content is empty");
            setCachedPassword(password);
            replacement.setAttribute("data-pinpad-decrypted", "1");
            document.getElementById("encrypted-content").replaceWith(replacement);
            jumpToHashIfPresent();
        } catch (err) {
            clearCachedPassword();
            if (!options.silent) {
                showStatus("Error!");
            }
            console.error("Decryption failed:", err);
            if (!options.silent) {
                input.value = "";
                input.focus();
            }
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
    const cachedPassword = getCachedPassword();
    if (cachedPassword) {
        handleAccept(cachedPassword, { silent: true });
        return;
    }

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
