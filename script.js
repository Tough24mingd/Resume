// David Ming — Resume Site
// script.js — tab switching + project tile detail views

document.addEventListener('DOMContentLoaded', () => {
  const tabs = document.querySelectorAll('.tab');
  const panels = document.querySelectorAll('.panel');
  const projectGrid = document.getElementById('project-grid');
  const projectDetail = document.getElementById('project-detail');
  const detailPanels = document.querySelectorAll('.detail-panel');
  const backLink = document.getElementById('back-to-projects');

  function showProjectGrid() {
    if (!projectGrid || !projectDetail) return;
    projectGrid.hidden = false;
    projectDetail.hidden = true;
    detailPanels.forEach((panel) => { panel.hidden = true; });
  }

  function showProjectDetail(name) {
    if (!projectGrid || !projectDetail) return;
    projectGrid.hidden = true;
    projectDetail.hidden = false;
    detailPanels.forEach((panel) => {
      panel.hidden = panel.id !== `detail-${name}`;
    });
  }

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
    if (name === 'projects') {
      showProjectGrid();
    }
  }

  tabs.forEach((tab) => {
    tab.addEventListener('click', () => {
      activate(tab.dataset.tab);
      history.replaceState(null, '', `#${tab.dataset.tab}`);
    });
  });

  document.querySelectorAll('.project-tile').forEach((tile) => {
    tile.addEventListener('click', () => {
      showProjectDetail(tile.dataset.project);
    });
  });

  if (backLink) {
    backLink.addEventListener('click', showProjectGrid);
  }

  const order = Array.from(tabs).map((t) => t.dataset.tab);
  const initial = window.location.hash.replace('#', '');
  if (order.includes(initial)) {
    activate(initial);
  }
});
