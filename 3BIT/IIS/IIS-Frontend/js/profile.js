import { baseUrl } from "../config/config.js";

document.addEventListener("DOMContentLoaded", function () {
    loadProfileInfo();
    handleProfileUpdate();
    handlePasswordUpdate();
    handleAccountDeletion();
    addShowPasswordToggles();
});

// Load the user's profile information into the form
function loadProfileInfo() {
    const user = JSON.parse(localStorage.getItem('user'));
    if (!user) {
        alert("You must be logged in to access the profile page.");
        window.location.href = "/pages/index.html";
        return;
    }

    document.getElementById("name").value = user.name;
    document.getElementById("email").value = user.email;
    document.getElementById("picture").value = user.picture || '';
    document.getElementById("current-picture").src = user.picture || '/assets/default-avatar.png';
}

// Handle profile information update
function handleProfileUpdate() {
    const profileForm = document.getElementById("profile-form");
    profileForm.addEventListener("submit", function (event) {
        event.preventDefault();

        const name = document.getElementById("name").value.trim();
        const email = document.getElementById("email").value.trim();
        const picture = document.getElementById("picture").value.trim();
        const user = JSON.parse(localStorage.getItem('user'));

        fetch(`${baseUrl}/api/users/${user.id}`, {
            method: "PUT",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ name, email, picture })
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error("Failed to update profile information.");
                }
                return response.json();
            })
            .then(updatedUser => {
                // Update local storage
                localStorage.setItem("user", JSON.stringify(updatedUser));
                document.getElementById("current-picture").src = updatedUser.picture || '/assets/default-avatar.png';
                alert("Profile updated successfully!");
            })
            .catch(error => {
                console.error("Error updating profile information:", error);
                alert("Error updating profile information.");
            });
    });
}

// Handle password update
function handlePasswordUpdate() {
    const passwordForm = document.getElementById("password-form");
    passwordForm.addEventListener("submit", function (event) {
        event.preventDefault();

        const currentPassword = document.getElementById("current-password").value.trim();
        const newPassword = document.getElementById("new-password").value.trim();
        const confirmPassword = document.getElementById("confirm-new-password").value.trim();
        const user = JSON.parse(localStorage.getItem('user'));

        if (newPassword !== confirmPassword) {
            alert("New passwords do not match. Please try again.");
            return;
        }

        fetch(`${baseUrl}/api/users/${user.id}/password`, {
            method: "PUT",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ currentPassword, newPassword })
        })
            .then(response => {
                if (!response.ok) {
                    if (response.status === 400) {
                        throw new Error("Current password is incorrect.");
                    }
                    throw new Error("Failed to update password.");
                }
                return response.json();
            })
            .then(() => {
                alert("Password updated successfully!");
                document.getElementById("current-password").value = "";
                document.getElementById("new-password").value = "";
                document.getElementById("confirm-new-password").value = "";
            })
            .catch(error => {
                console.error("Error updating password:", error);
                alert(error.message);
            });
    });
}

// Handle account deletion
function handleAccountDeletion() {
    const deleteButton = document.getElementById("delete-account");

    deleteButton.addEventListener("click", function () {
        if (confirm("Are you sure you want to delete your account? This action cannot be undone.")) {
            const user = JSON.parse(localStorage.getItem("user"));

            fetch(`${baseUrl}/api/users/${user.id}`, {
                method: "DELETE",
                headers: { "Content-Type": "application/json" }
            })
                .then(response => {
                    if (!response.ok) {
                        throw new Error("Failed to delete account.");
                    }
                    return response.json();
                })
                .then(() => {
                    alert("Your account has been deleted.");
                    localStorage.removeItem("user");
                    window.location.href = "/pages/index.html";
                })
                .catch(error => {
                    console.error("Error deleting account:", error);
                    alert("Error deleting account.");
                });
        }
    });
}

// Add "Show Password" toggle functionality
function addShowPasswordToggles() {
    const toggleButtons = document.querySelectorAll(".toggle-password");

    toggleButtons.forEach(button => {
        const input = document.getElementById(button.dataset.target);

        button.addEventListener("mousedown", function () {
            input.type = "text";
        });

        button.addEventListener("mouseup", function () {
            input.type = "password";
            this.blur(); // Remove focus from the button to stop the visual state
        });

        button.addEventListener("mouseleave", function () {
            input.type = "password";
            this.blur(); // Remove focus from the button to stop the visual state
        });

        // For mobile devices
        button.addEventListener("touchstart", function () {
            input.type = "text";
        });

        button.addEventListener("touchend", function () {
            input.type = "password";
            this.blur(); // Remove focus from the button to stop the visual state
        });
    });
}
