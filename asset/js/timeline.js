/**
 * Click-and-drag panning, and "there's more" edge arrows, for the
 * session_timeline() macro's horizontal timeline (calendar page).
 *
 * `.timeline-scroll` (the actual overflow-x:auto element, native scrollbar
 * hidden in CSS) is what's dragged and scrolled: mouse-dragging anywhere on
 * it pans the timeline, including when the drag starts on a card's
 * title/badge link -- only an actual drag (pointer moved) suppresses that
 * link's click, so a plain click still navigates to the quest. Touch/
 * trackpad users keep native scrolling -- only mouse pointers trigger this
 * custom drag. `.timeline-arrow-left`/`-right` (siblings of
 * `.timeline-scroll`, absolutely positioned against the outer `.timeline`)
 * are shown/hidden to match whether there's more content in that direction.
 *
 * Bootstrap: hooks into Material's document$ observable for instant-
 * navigation re-init, and also runs on DOMContentLoaded / immediately for
 * hard loads. A data attribute guards against double-initialisation.
 */
function initTimelineDrag() {
    document.querySelectorAll(".timeline").forEach(function (timeline) {
        if (timeline.dataset.dragInit === "1") return;
        timeline.dataset.dragInit = "1";

        const scroller = timeline.querySelector(".timeline-scroll");
        const leftArrow = timeline.querySelector(".timeline-arrow-left");
        const rightArrow = timeline.querySelector(".timeline-arrow-right");
        if (!scroller) return;

        function updateArrows() {
            const max = scroller.scrollWidth - scroller.clientWidth;
            if (leftArrow) leftArrow.classList.toggle("visible", scroller.scrollLeft > 1);
            if (rightArrow) rightArrow.classList.toggle("visible", scroller.scrollLeft < max - 1);
        }

        // Each arrow, when visible, scrolls the timeline by most of a
        // viewport's width in that direction.
        if (leftArrow) {
            leftArrow.addEventListener("click", function () {
                scroller.scrollBy({left: -scroller.clientWidth * 0.8, behavior: "smooth"});
            });
        }
        if (rightArrow) {
            rightArrow.addEventListener("click", function () {
                scroller.scrollBy({left: scroller.clientWidth * 0.8, behavior: "smooth"});
            });
        }

        // Blocks the browser's own native drag-and-drop (link/image ghost
        // dragging) once an actual drag is under way (see pointermove
        // below) -- dragstart fires from the same gesture that triggers
        // it, so preventing it here doesn't touch a plain click at all.
        scroller.addEventListener("dragstart", function (event) {
            event.preventDefault();
        });

        // Tracked from pointerdown, but nothing (capture, the dragging
        // class, scrolling) actually starts until pointermove sees the
        // pointer cross DRAG_THRESHOLD px -- a plain press-and-release
        // never calls setPointerCapture/preventDefault, so it can't
        // interfere with that click reaching the link underneath it.
        const DRAG_THRESHOLD = 4;
        let pointerId = null;
        let pressX = 0;
        let startScroll = 0;
        let dragging = false;

        scroller.addEventListener("pointerdown", function (event) {
            if (event.pointerType !== "mouse") return;
            pointerId = event.pointerId;
            pressX = event.clientX;
            startScroll = scroller.scrollLeft;
            dragging = false;
        });

        scroller.addEventListener("pointermove", function (event) {
            if (pointerId === null || event.pointerId !== pointerId) return;
            const dx = event.clientX - pressX;
            if (!dragging) {
                if (Math.abs(dx) < DRAG_THRESHOLD) return;
                dragging = true;
                scroller.setPointerCapture(pointerId);
                scroller.classList.add("timeline-dragging");
            }
            scroller.scrollLeft = startScroll - dx;
        });

        function endPress() {
            pointerId = null;
            scroller.classList.remove("timeline-dragging");
        }

        scroller.addEventListener("pointerup", endPress);
        scroller.addEventListener("pointercancel", endPress);

        // A drag that actually crossed the threshold shouldn't also fire a
        // click on whatever card it passed over or ended on.
        scroller.addEventListener("click", function (event) {
            if (dragging) {
                dragging = false;
                event.preventDefault();
                event.stopPropagation();
            }
        }, true);

        // Left/Right arrow keys pan the timeline like the edge-arrow
        // buttons, once the scroller has focus (it's tabindex="0").
        scroller.addEventListener("keydown", function (event) {
            if (event.key === "ArrowLeft") {
                event.preventDefault();
                scroller.scrollBy({left: -scroller.clientWidth * 0.8, behavior: "smooth"});
            } else if (event.key === "ArrowRight") {
                event.preventDefault();
                scroller.scrollBy({left: scroller.clientWidth * 0.8, behavior: "smooth"});
            }
        });

        scroller.addEventListener("scroll", updateArrows);
        window.addEventListener("resize", updateArrows);
        updateArrows();
    });
}

// ── Bootstrap ────────────────────────────────────────────────────────────────
if (typeof document$ !== "undefined") {
    document$.subscribe(() => initTimelineDrag());
}
if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initTimelineDrag);
} else {
    initTimelineDrag();
}
