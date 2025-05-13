import { baseUrl } from "../config/config.js";

export async function validateUser() {
    const user = JSON.parse(localStorage.getItem('user'));

    if (!user || !user.id) {
        // Redirect to login if no user is found in local storage
        window.location.href = '/pages/index.html';
        return;
    }

    try {
        const response = await fetch(`${baseUrl}/api/auth/validate`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
                'User-ID': user.id
            }
        });

        if (response.ok) {
            const data = await response.json();
            // Update local storage with the most recent user data
            localStorage.setItem('user', JSON.stringify(data));
        } else {
            throw new Error('User validation failed. Redirecting to login...');
        }
    } catch (error) {
        console.error('Error validating user:', error);
        localStorage.removeItem('user');
        window.location.href = '/pages/index.html';
    }
}
