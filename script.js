// David Ming — Resume Site
// script.js — tab switching + sheet indicator

document.addEventListener('DOMContentLoaded', () => {
  const tabs = document.querySelectorAll('.tab');
  const panels = document.querySelectorAll('.panel');
  const sheetIndicator = document.getElementById('sheet-indicator');
  const order = Array.from(tabs).map((t) => t.dataset.tab);

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
    const index = order.indexOf(name);
    if (sheetIndicator && index !== -1) {
      sheetIndicator.textContent = `${String(index + 1).padStart(2, '0')} / ${String(order.length).padStart(2, '0')}`;
    }
  }

  tabs.forEach((tab) => {
    tab.addEventListener('click', () => {
      activate(tab.dataset.tab);
      history.replaceState(null, '', `#${tab.dataset.tab}`);
    });
  });

  const initial = window.location.hash.replace('#', '');
  if (order.includes(initial)) {
    activate(initial);
  }
});
