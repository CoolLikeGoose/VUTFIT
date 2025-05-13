import { validateUser } from './validateUser.js';

// Inactivity timeout (in milliseconds) - 15 minutes
const INACTIVITY_TIMEOUT = 15 * 60 * 1000; // 15 minutes

// Store the inactivity timeout ID globally so it can be accessed and reset
let inactivityTimeoutId;

// Function to log out the user after inactivity
function logoutUser() {
    alert("You have been logged out due to inactivity.");
    localStorage.removeItem('user');
    window.location.href = '/pages/index.html';
}

// Function to reset inactivity timer
function resetInactivityTimer() {
    // Clear the previous timer
    if (inactivityTimeoutId) {
        clearTimeout(inactivityTimeoutId);
    }

    // Set a new timer for the specified inactivity period
    inactivityTimeoutId = setTimeout(logoutUser, INACTIVITY_TIMEOUT);
}

// Function to add activity listeners only when user is logged in
function addActivityListeners() {
    document.addEventListener('mousemove', resetInactivityTimer);
    document.addEventListener('keydown', resetInactivityTimer);
    document.addEventListener('scroll', resetInactivityTimer);
    document.addEventListener('click', resetInactivityTimer);
}

// Function to remove activity listeners
function removeActivityListeners() {
    document.removeEventListener('mousemove', resetInactivityTimer);
    document.removeEventListener('keydown', resetInactivityTimer);
    document.removeEventListener('scroll', resetInactivityTimer);
    document.removeEventListener('click', resetInactivityTimer);
}

export async function loadHeader() {
    // Load the header first, regardless of user login status
    fetch('/pages/header.html')
        .then(response => response.text())
        .then(async (data) => {
            document.getElementById('header').innerHTML = data;

            // Check if user is logged in
            const user = JSON.parse(localStorage.getItem('user'));
            if (user) {
                try {
                    // Validate user only if user data exists in local storage
                    await validateUser();

                    // Update UI with user info after successful validation
                    const updatedUser = JSON.parse(localStorage.getItem('user'));
                    document.getElementById('my-conferences-link').classList.remove('d-none');
                    document.getElementById('user-info').classList.remove('d-none');
                    document.getElementById('user-name').textContent = `Hello, ${updatedUser.name}`;

                    // If user has a profile picture, display it
                    if (updatedUser.picture) {
                        const userPictureElement = document.getElementById('user-picture');
                        userPictureElement.src = updatedUser.picture;
                        userPictureElement.classList.remove('d-none');
                    }

                    document.getElementById('login-link').classList.add('d-none');
                    document.getElementById('signup-link').classList.add('d-none');
                    document.getElementById('logout-link').addEventListener('click', function () {
                        localStorage.removeItem('user');
                        removeActivityListeners(); // Remove activity listeners when logging out
                        window.location.href = '/pages/index.html';
                    });

                    // Show admin link if user is an admin
                    if (updatedUser['role'] === 'admin') {
                        document.getElementById('admin-link').classList.remove('d-none');
                    }

                    // Start/reset the inactivity timer when the user is successfully logged in
                    addActivityListeners();
                    resetInactivityTimer();

                } catch (error) {
                    console.error('Error during user validation:', error);
                }
            }
        })
        .catch(error => console.error('Error loading header:', error));
}
