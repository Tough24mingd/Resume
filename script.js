// David Ming — Resume Site
// script.js — reserved for future interactivity

// Highlight the current year in the footer automatically
document.addEventListener('DOMContentLoaded', () => {
  const footer = document.querySelector('footer span:last-child');
  if (footer) {
    footer.textContent = `David Ming · ${new Date().getFullYear()}`;
  }
});
