// David Ming — Resume Site
// script.js — tab switching

document.addEventListener('DOMContentLoaded', () => {
  const tabs = document.querySelectorAll('.tab');
  const panels = document.querySelectorAll('.panel');

  function activate(name) {
    tabs.forEach((tab) => {
      const isActive = tab.dataset.tab === name;
      tab.classList.toggle('active', isActive);
      tab.setAttribute('aria-selected', isActive ? 'true' : 'false');
    });
    panels.forEach((panel) => {
      const isActive = panel.id === name;
      panel.classList.toggle('active', isActive);
      panel.hidden = !isActive;
    });
  }

  tabs.forEach((tab) => {
    tab.addEventListener('click', () => {
      activate(tab.dataset.tab);
      history.replaceState(null, '', `#${tab.dataset.tab}`);
    });
  });

  const order = Array.from(tabs).map((t) => t.dataset.tab);
  const initial = window.location.hash.replace('#', '');
  if (order.includes(initial)) {
    activate(initial);
  }
});
