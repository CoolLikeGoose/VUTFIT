export function initNavBar(confId) {
    document.querySelectorAll('.sidebar a').forEach(link => {
        if (confId !== null) {
            link.classList.remove('disabled');
            link.style.cssText = '';
        }

        link.addEventListener('click', event => {
            event.preventDefault();
            const targetId = link.getAttribute('data-target');

            document.querySelectorAll('.section').forEach(section => {
                section.style.display = 'none';
            });

            const targetSection = document.getElementById(targetId);
            if (targetSection) {
                targetSection.style.display = 'block';
            }
        });
    });

    const firstSection = document.querySelector('.section');
    if (firstSection) {
        firstSection.style.display = 'block';
    }
}
