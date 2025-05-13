/**
 * Help JS code which sets up the flag when the page is scrolled.
 * 
 * @author Jiri Hynek
 */

/**
 * the flag
 */
let lastScroll = false;

window.addEventListener("load", () => {

    window.addEventListener("scroll", () => {
        if(window.scrollY > 0) {
            if(lastScroll === false) {
                document.documentElement.classList.toggle("scrolled");
                lastScroll = true;
            }
        } else {
            if(lastScroll === true) {
                document.documentElement.classList.toggle("scrolled");
                lastScroll = false;
            }
        }
    })
});