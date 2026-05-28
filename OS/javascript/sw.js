/* SPDX-License-Identifier: GPL-2.0-only 
* Carl-OS/.js/sw.js
* /* Copyright (C) MDFJ 2026
* MDFJ Project: CarlOS (Part 3 of Carl)
* License: GNU General Public License v2.0. */

const CACHE = 'carlos-v12-cache-v1';
const PRECACHE = ['/', '/newtab'];

self.addEventListener('install', e => {
  e.waitUntil(
    caches.open(CACHE).then(c => c.addAll(PRECACHE)).then(() => self.skipWaiting())
  );
});

self.addEventListener('activate', e => {
  e.waitUntil(
    caches.keys().then(keys =>
      Promise.all(keys.filter(k => k !== CACHE).map(k => caches.delete(k)))
    ).then(() => self.clients.claim())
  );
});

self.addEventListener('fetch', e => {
  if (e.request.url.includes('/proxy')) return;
  e.respondWith(
    fetch(e.request).catch(() => caches.match(e.request))
  );
});
