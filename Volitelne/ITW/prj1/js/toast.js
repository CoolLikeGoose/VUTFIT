/**
 * Help JS code which opens the toast.
 * 
 * @author Jiri Hynek
 */
function toast() {
    let btn = document.getElementById("manny");
    
    if(btn) {
        btn.classList.toggle("open");

        // disable button needs to be scheduled since we want to prevent default validation
        setTimeout(() => {
            document.querySelectorAll("input[type='submit']")?.forEach((btn) => btn.disabled = true);
        }, 0);

        // note: you could process the form here and call an async function to send the values to the server

        setTimeout(() => {
            document.getElementById("manny")?.classList.toggle("open");
            document.querySelectorAll("input[type='submit']")?.forEach((btn) => btn.disabled = null);
        }, 2000);
    }
}