#!/usr/bin/env python3
"""Tools to encrypt generated solution section HTML."""

import argparse
import base64
import os

from bs4 import BeautifulSoup
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

from quest import PROJECT_ROOT, Quest

_encryption_template = """
<main id="encrypted-content" class="md-content" data-md-component="content">

<div class="md-content" data-md-component="content">

    <article class="md-content__inner md-typeset">
        <div class="badge frame"> <img src="https://uab-p2.github.io/sol/asset/img/lock.png"> </div>
        <br/>
    
        <div class="pinpad-container" style="max-width: 300px; margin: 0 auto; padding: 20px; border: 2px solid #333; border-radius: 4px;">
            <label style="display: block; margin-bottom: 15px; font-weight: bold;">{title}</label>
            
            <div id="led-display" style="font-family: 'Courier New', monospace; font-size: 18px; padding: 10px; margin-bottom: 15px; background-color: #f0f0f0; border: 1px solid #999; border-radius: 2px; min-height: 30px; word-break: break-all;"></div>
            
            <div class="pinpad-grid" style="display: grid; grid-template-columns: repeat(4, 1fr); gap: 5px; margin-bottom: 10px;">
                <button class="hex-btn" data-key="0" style="padding: 10px; border: 1px solid #666; cursor: pointer;">0</button>
                <button class="hex-btn" data-key="1" style="padding: 10px; border: 1px solid #666; cursor: pointer;">1</button>
                <button class="hex-btn" data-key="2" style="padding: 10px; border: 1px solid #666; cursor: pointer;">2</button>
                <button class="hex-btn" data-key="3" style="padding: 10px; border: 1px solid #666; cursor: pointer;">3</button>
                <button class="hex-btn" data-key="4" style="padding: 10px; border: 1px solid #666; cursor: pointer;">4</button>
                <button class="hex-btn" data-key="5" style="padding: 10px; border: 1px solid #666; cursor: pointer;">5</button>
                <button class="hex-btn" data-key="6" style="padding: 10px; border: 1px solid #666; cursor: pointer;">6</button>
                <button class="hex-btn" data-key="7" style="padding: 10px; border: 1px solid #666; cursor: pointer;">7</button>
                <button class="hex-btn" data-key="8" style="padding: 10px; border: 1px solid #666; cursor: pointer;">8</button>
                <button class="hex-btn" data-key="9" style="padding: 10px; border: 1px solid #666; cursor: pointer;">9</button>
                <button class="hex-btn" data-key="A" style="padding: 10px; border: 1px solid #666; cursor: pointer;">A</button>
                <button class="hex-btn" data-key="B" style="padding: 10px; border: 1px solid #666; cursor: pointer;">B</button>
                <button class="hex-btn" data-key="C" style="padding: 10px; border: 1px solid #666; cursor: pointer;">C</button>
                <button class="hex-btn" data-key="D" style="padding: 10px; border: 1px solid #666; cursor: pointer;">D</button>
                <button class="hex-btn" data-key="E" style="padding: 10px; border: 1px solid #666; cursor: pointer;">E</button>
                <button class="hex-btn" data-key="F" style="padding: 10px; border: 1px solid #666; cursor: pointer;">F</button>
            </div>
            
            <div style="display: grid; grid-template-columns: repeat(3, 1fr); gap: 5px;">
                <button id="backspace-btn" style="padding: 10px; border: 1px solid #666; cursor: pointer; grid-column: span 1;">⌫</button>
                <button id="decrypt-button" style="padding: 10px; border: 1px solid #666; cursor: pointer; grid-column: span 1;">▶</button>
            </div>
            
            <p id="decrypt-status" hidden style="margin-top: 10px; text-align: center; font-size: 14px;"></p>
        </div>
    </article>
</div>

<script id="template" type="text/plain">
{encrypted_content}
</script>

<script>
let passwordValue = "";

async function decryptTemplate(password) {
    // Read Base64 from the script tag
    const b64 = document.getElementById("template").textContent.trim();

    // Decode Base64
    const bytes = Uint8Array.from(atob(b64), c => c.charCodeAt(0));

    // Split into components
    const salt = bytes.slice(0, 16);
    const iv = bytes.slice(16, 28);
    const ciphertext = bytes.slice(28);

    // Import password
    const passwordKey = await crypto.subtle.importKey(
        "raw",
        new TextEncoder().encode(password),
        "PBKDF2",
        false,
        ["deriveKey"]
    );

    // Derive AES key
    const aesKey = await crypto.subtle.deriveKey(
        {
            name: "PBKDF2",
            salt,
            iterations: 1000000,
            hash: "SHA-256",
        },
        passwordKey,
        {
            name: "AES-GCM",
            length: 256,
        },
        false,
        ["decrypt"]
    );

    // Decrypt
    const plaintext = await crypto.subtle.decrypt(
        {
            name: "AES-GCM",
            iv,
        },
        aesKey,
        ciphertext
    );

    return new TextDecoder().decode(plaintext);
}

function updateDisplay() {
    document.getElementById("led-display").textContent = passwordValue;
}

function handleHexKeyPress(key) {
    passwordValue += key.toUpperCase();
    updateDisplay();
    
    const status = document.getElementById("decrypt-status");
    status.hidden = true;
}

function handleHexTextInput(text) {
    const hexChars = Array.from(text).filter(char => /[0-9a-f]/i.test(char));

    if (hexChars.length === 0) {
        return;
    }

    hexChars.forEach(handleHexKeyPress);
}

function handleBackspace() {
    passwordValue = passwordValue.slice(0, -1);
    updateDisplay();
}

async function handleDecrypt() {
    const status = document.getElementById("decrypt-status");

    status.hidden = false;
    status.textContent = "...";

    try {
        const decryptedHtml = await decryptTemplate(passwordValue.toLowerCase());
        const parser = new DOMParser();
        const parsed = parser.parseFromString(decryptedHtml, "text/html");
        const replacement = parsed.body.firstElementChild;

        if (!replacement) {
            throw new Error("Decrypted content is empty");
        }

        const encryptedContent = document.getElementById("encrypted-content");
        encryptedContent.replaceWith(replacement);
    } catch (err) {
        status.textContent = "Error!";
        console.error("Decryption failed:", err);
        passwordValue = "";
        updateDisplay();
    }
}

function handleDocumentKeydown(event) {
    const target = event.target;
    const isEditableTarget = target instanceof HTMLElement && (
        target.isContentEditable ||
        ["INPUT", "TEXTAREA", "SELECT"].includes(target.tagName)
    );

    if (isEditableTarget) {
        return;
    }

    if (event.ctrlKey || event.metaKey) {
        if (event.key.toLowerCase() === "v") {
            return;
        }
        return;
    }

    if (/^[0-9a-f]$/i.test(event.key)) {
        event.preventDefault();
        event.stopPropagation();
        handleHexKeyPress(event.key);
        return;
    }

    if (event.key === "Backspace") {
        event.preventDefault();
        event.stopPropagation();
        handleBackspace();
        return;
    }

    if (event.key === "Enter") {
        event.preventDefault();
        event.stopPropagation();
        handleDecrypt();
    }
}

function handleDocumentPaste(event) {
    const target = event.target;
    const isEditableTarget = target instanceof HTMLElement && (
        target.isContentEditable ||
        ["INPUT", "TEXTAREA", "SELECT"].includes(target.tagName)
    );

    if (isEditableTarget) {
        return;
    }

    const pastedText = event.clipboardData?.getData("text") ?? "";

    if (!/[0-9a-f]/i.test(pastedText)) {
        return;
    }

    event.preventDefault();
    event.stopPropagation();
    handleHexTextInput(pastedText);
}

// Attach handlers
document.querySelectorAll(".hex-btn").forEach(btn => {
    btn.addEventListener("click", () => handleHexKeyPress(btn.dataset.key));
});

document.getElementById("backspace-btn").addEventListener("click", handleBackspace);
document.getElementById("decrypt-button").addEventListener("click", handleDecrypt);
window.addEventListener("keydown", handleDocumentKeydown, true);
window.addEventListener("paste", handleDocumentPaste, true);
</script>

</main>
"""


def encrypt_solution(solution: Quest) -> str:
    """Encrypt the solution index.html file with the given password."""
    # Normalize password
    password = solution.secret.lower().strip()
    assert all("a" <= c <= "f" or "0" <= c <= "9" for c in password), (
        "Secrets should contain only lowercase letters, numbers, and the characters a-h "
    )

    # Get original HTML content
    html_path = PROJECT_ROOT / "doc" / "guide" / "html" / "auto" \
                / f"solution_{os.path.basename(solution.module_path)}" / "index.html"
    with open(html_path, "r") as f:
        html = f.read()
    soup = BeautifulSoup(html, "html.parser")

    # Replace the main content with the encrypted content + hex pin pad
    content_div = soup.find(
        "main",
        class_="md-main",
        attrs={"data-md-component": "main"},
    )
    if content_div is None:
        print("Seems like the HTML has already been processed and encrypted. Skipping.")
        return

    html = _encryption_template
    html = html.replace("{title}", os.path.basename(solution.title))
    html = html.replace("{encrypted_content}", encrypt_contents(content_div.prettify(), password))
    replacement_div = BeautifulSoup(html, "html.parser")
    content_div.replace_with(replacement_div)

    # Write the modified HTML back to the file
    with open(html_path, "w") as f:
        f.write(str(soup))


def encrypt_contents(contents, password):
    """Encrypt a blob of contents using AES-GCM with a password.
    :return: Base64-encoded encrypted blob, salt || iv || cyphertext
    """
    # Random parameters
    salt = os.urandom(16)
    iv = os.urandom(12)

    # Derive 256-bit AES key
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=32,
        salt=salt,
        iterations=1000000,
    )
    key = kdf.derive(password.encode("utf-8"))

    # Encrypt
    aesgcm = AESGCM(key)
    ciphertext = aesgcm.encrypt(
        iv,
        contents.encode("utf-8"),
        None,  # associated data
    )

    # Output: salt || iv || ciphertext
    blob = salt + iv + ciphertext
    return base64.b64encode(blob).decode("ascii")


def encrypt_solutions():
    """Encrypt all solution index.html files in the docs/solutions directory."""
    solutions: list[Quest] = Quest.solutions()
    for solution in solutions:
        print(f"Encrypting {solution}...")
        encrypt_solution(solution=solution)

def show_secrets():
    solutions: list[Quest] = Quest.solutions()
    for solution in solutions:
        print(f"Secret for '{solution!r}': {solution.secret}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Encrypt solution index.html files.")
    parser.add_argument("--secrets", action="store_true", help="If enabled, the secrets for all solutions are shown")
    options = parser.parse_args()
    if options.secrets:
        show_secrets()
    else:
        encrypt_solutions()
