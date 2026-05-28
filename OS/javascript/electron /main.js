const { app, BrowserWindow, shell, ipcMain, Menu } = require('electron');
const { spawn, execSync } = require('child_process');
const path = require('path');
const http = require('http');
const os = require('os');

const PORT = 5173;
let mainWindow = null;
let serverProcess = null;
let compileOK = false;

// ── Compile main.c ─────────────────────────────────────────────────────────
function compileC() {
  const src = path.join(__dirname, '..', 'main.c');
  const out = path.join(__dirname, '..', 'main');
  try {
    execSync(`gcc "${src}" -o "${out}"`, { stdio: 'pipe' });
    compileOK = true;
    console.log('[CarlOS] main.c compilado OK');
  } catch (e) {
    console.error('[CarlOS] Error compilando main.c:', e.stderr?.toString());
    compileOK = false;
  }
}

// ── Start the Express/Socket.IO server ─────────────────────────────────────
function startServer() {
  const serverPath = path.join(__dirname, '..', 'server.js');
  serverProcess = spawn(process.execPath, [serverPath], {
    cwd: path.join(__dirname, '..'),
    env: { ...process.env, PORT: String(PORT), ELECTRON: '1' },
    stdio: 'inherit',
  });
  serverProcess.on('error', err => console.error('[CarlOS] server error:', err));
  serverProcess.on('exit', code => console.log('[CarlOS] server exit:', code));
}

// ── Wait until the server responds ─────────────────────────────────────────
function waitForServer(url, retries, delay) {
  return new Promise((resolve, reject) => {
    const attempt = n => {
      http.get(url, res => {
        res.resume();
        resolve();
      }).on('error', () => {
        if (n <= 0) return reject(new Error('Server did not start'));
        setTimeout(() => attempt(n - 1), delay);
      });
    };
    attempt(retries);
  });
}

// ── Create the main window ─────────────────────────────────────────────────
function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1280,
    height: 800,
    minWidth: 900,
    minHeight: 600,
    title: 'Carl OS v12.02 LTS',
    icon: path.join(__dirname, '..', 'public', 'icon-512.png'),
    backgroundColor: '#0d0d1a',
    autoHideMenuBar: true,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      sandbox: false,
      webviewTag: true,
      allowRunningInsecureContent: false,
    },
  });

  Menu.setApplicationMenu(null);

  mainWindow.loadURL(`http://localhost:${PORT}`);

  mainWindow.webContents.setWindowOpenHandler(({ url }) => {
    shell.openExternal(url);
    return { action: 'deny' };
  });

  mainWindow.on('closed', () => { mainWindow = null; });
}

// ── App lifecycle ──────────────────────────────────────────────────────────
app.whenReady().then(async () => {
  compileC();
  startServer();
  try {
    await waitForServer(`http://localhost:${PORT}`, 30, 500);
    createWindow();
  } catch (e) {
    console.error('[CarlOS] El servidor no respondió a tiempo.');
    app.quit();
  }

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on('window-all-closed', () => {
  if (serverProcess) serverProcess.kill();
  if (process.platform !== 'darwin') app.quit();
});

app.on('before-quit', () => {
  if (serverProcess) serverProcess.kill();
});
