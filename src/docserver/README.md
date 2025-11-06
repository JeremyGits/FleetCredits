# Fleet Credits Documentation Server

## Quick Start

```bash
npm install
npm run dev
```

This will start a development server at `http://localhost:3001` with live reload enabled.

## Structure

```
docserver/
├── index.html              # Main overview page
├── assets/
│   ├── css/
│   │   └── main.css       # Styles
│   └── js/
│       └── main.js        # Search & navigation
├── pages/                  # Documentation pages
│   ├── architecture.html
│   ├── rpc-overview.html
│   └── ... (more pages)
└── package.json           # Node.js dependencies
```

## Development

- **Live Reload**: Automatically reloads when files change
- **Port**: Default port is 3001 (configurable in package.json)
- **Search**: Sidebar search filters navigation items
- **Navigation**: Consistent sidebar across all pages

## Adding New Pages

1. Create new HTML file in `pages/` directory
2. Copy sidebar structure from existing pages
3. Update navigation links in `index.html` and other pages
4. Page will auto-reload during development

## Production

For production deployment, you can use any static file server:
- GitHub Pages
- Netlify
- Vercel
- Apache/Nginx

All files are static HTML/CSS/JS - no build step required!
