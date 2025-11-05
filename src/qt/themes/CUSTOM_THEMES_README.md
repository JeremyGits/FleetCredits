# 🎨 Dogecoin Core Custom Themes Guide

Welcome to the **Custom Themes System** for Dogecoin Core! This guide will help you create, share, and use custom themes to personalize your Dogecoin experience.

## 📁 Theme Directory Structure

```
src/qt/themes/
├── builtin/           # Built-in color themes (Light, Dark, Dogecoin, etc.)
├── css/              # CSS-based themes (Matrix, Cyberpunk, etc.)
├── custom/           # Your custom themes go here!
├── templates/        # Theme templates to start from
└── shared/          # Imported themes from other users
```

## 🚀 Quick Start - Create Your First Theme

### Method 1: Drag & Drop (Easiest!)

1. **Create a theme folder**: Create a new folder in `src/qt/themes/custom/` with your theme name
2. **Add CSS file**: Create a CSS file with the same name as your folder (e.g., `mytheme.css`)
3. **Write your styles**: Copy and modify styles from existing themes
4. **Restart Dogecoin**: Your theme will appear in the theme selector!

**Example:**
```
src/qt/themes/custom/myawesome/
├── myawesome.css
└── theme.json (optional)
```

### Method 2: Use a Template

1. **Choose a template**: Copy a folder from `src/qt/themes/templates/`
2. **Customize**: Modify the CSS and metadata files
3. **Save**: The theme will be automatically discovered

### Method 3: Visual Theme Builder (Coming Soon!)

Use the built-in theme builder in Dogecoin Core:
1. Go to **Settings → Themes → Create New Theme**
2. Use the visual editor to customize colors and styles
3. Preview your changes in real-time
4. Save and share your theme!

## 📝 CSS Theme Structure

### Basic CSS File Structure

```css
/* Your Theme Name - Description */
/* Created by YourName */

/* Main window styling */
QMainWindow {
    background-color: #your-color;
    color: #your-text-color;
    font-family: 'Your Font', sans-serif;
    font-size: 12px;
}

/* Base widget styling */
QWidget {
    background-color: #your-background;
    color: #your-text;
    border: none;
}

/* Buttons */
QPushButton {
    background-color: #button-bg;
    color: #button-text;
    border: 1px solid #button-border;
    border-radius: 4px;
    padding: 8px 16px;
}

QPushButton:hover {
    background-color: #button-hover;
}

QPushButton:pressed {
    background-color: #button-pressed;
}

/* Input fields */
QLineEdit, QTextEdit, QSpinBox {
    background-color: #input-bg;
    color: #input-text;
    border: 1px solid #input-border;
    border-radius: 4px;
    padding: 6px 12px;
}

QLineEdit:focus, QTextEdit:focus {
    border-color: #focus-color;
}

/* Add more styles as needed... */
```

### Essential CSS Selectors

| Selector | Purpose | Example |
|----------|---------|---------|
| `QMainWindow` | Main application window | Background, text color |
| `QWidget` | All widgets (base) | Default colors, fonts |
| `QPushButton` | All buttons | Button styling |
| `QLineEdit` | Text input fields | Input field styling |
| `QComboBox` | Dropdown menus | Dropdown styling |
| `QMenu` | Context menus | Menu styling |
| `QScrollBar` | Scrollbars | Scrollbar styling |
| `QTabWidget` | Tabbed interfaces | Tab styling |

### Special Dogecoin Selectors

```css
/* Modern UI Container */
QWidget#modernContainer {
    background-color: #your-color;
}

/* Navigation Sidebar */
QWidget#modernNavigation {
    background-color: #nav-color;
    border-right: 1px solid #nav-border;
}

/* Navigation Buttons */
QPushButton#navButton {
    background-color: transparent;
    color: #nav-text;
    text-align: left;
    padding: 12px 16px;
}

QPushButton#navButton:hover {
    background-color: #nav-hover;
}

QPushButton#navButton:checked {
    background-color: #nav-active;
}
```

## 📋 Theme Metadata (Optional)

Create a `theme.json` file to add metadata to your theme:

```json
{
  "name": "My Awesome Theme",
  "description": "A beautiful custom theme for Dogecoin",
  "author": "YourName",
  "version": "1.0",
  "type": "custom",
  "preview": "preview.png",
  "tags": ["dark", "neon", "gaming"],
  "compatibility": "1.14.99+",
  "customProperties": {
    "primaryColor": "#ff6b35",
    "backgroundColor": "#1a1a1a",
    "textColor": "#ffffff"
  }
}
```

### Metadata Fields

| Field | Required | Description |
|-------|----------|-------------|
| `name` | Yes | Display name of your theme |
| `description` | Yes | Short description |
| `author` | Yes | Your name or username |
| `version` | No | Version number (default: "1.0") |
| `type` | No | Theme type: "custom", "template", "shared" |
| `preview` | No | Path to preview image |
| `tags` | No | Array of tags for categorization |
| `compatibility` | No | Minimum Dogecoin version |
| `customProperties` | No | Custom color properties |

## 🎨 Color Schemes & Inspiration

### Popular Color Schemes

#### Dark Themes
```css
/* Professional Dark */
background: #1a1a1a
text: #ffffff
accent: #0d6efd
border: #404040

/* Gaming Dark */
background: #0a0a0a
text: #00ff00
accent: #ff0080
border: #00ff00
```

#### Light Themes
```css
/* Clean Light */
background: #ffffff
text: #333333
accent: #007bff
border: #dee2e6

/* Warm Light */
background: #fff8dc
text: #8b4513
accent: #ff8c00
border: #daa520
```

#### Neon Themes
```css
/* Cyberpunk */
background: #000000
text: #ff00ff
accent: #00ffff
border: #ff00ff

/* Matrix */
background: #000000
text: #00ff00
accent: #00ff00
border: #00ff00
```

## 🔧 Advanced Customization

### Custom Fonts

```css
QMainWindow {
    font-family: 'Roboto', 'Segoe UI', Arial, sans-serif;
    font-size: 14px;
    font-weight: 500;
}
```

### Gradients

```css
QPushButton {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #ff6b35, stop:1 #f7931e);
}
```

### Shadows and Effects

```css
QFrame {
    border: 1px solid #ccc;
    border-radius: 8px;
    background-color: #ffffff;
}
```

### Animation (Limited Support)

```css
QPushButton:hover {
    background-color: #e9ecef;
    border-color: #adb5bd;
    /* Note: Qt CSS has limited animation support */
}
```

## 📤 Sharing Your Themes

### Export Your Theme

1. **Manual Export**: Copy your theme folder and share it
2. **Built-in Export**: Use the theme builder's export feature
3. **Create Theme Package**: Bundle CSS + metadata + preview image

### Theme Package Structure

```
MyAwesomeTheme/
├── myawesome.css          # Main CSS file
├── theme.json            # Metadata
├── preview.png           # Preview image (optional)
└── README.md            # Theme description (optional)
```

### Import Themes from Others

1. **Download theme package**
2. **Extract to `src/qt/themes/custom/`**
3. **Restart Dogecoin Core**
4. **Select theme from Settings → Themes**

## 🐛 Troubleshooting

### Common Issues

#### Theme Not Appearing
- ✅ Check folder name matches CSS filename
- ✅ Ensure CSS file is valid (no syntax errors)
- ✅ Restart Dogecoin Core after adding theme

#### Theme Looks Broken
- ✅ Check CSS syntax for missing semicolons or brackets
- ✅ Verify color values are valid (use hex: `#ff0000`)
- ✅ Test with a simple theme first

#### Performance Issues
- ✅ Avoid overly complex CSS
- ✅ Limit use of gradients and effects
- ✅ Test theme with different screen sizes

### Debug Mode

Enable debug logging to see theme loading:
```bash
./dogecoin-qt --debug=qt
```

## 🎯 Best Practices

### Design Guidelines

1. **Consistency**: Use consistent colors and spacing throughout
2. **Accessibility**: Ensure good contrast between text and background
3. **Readability**: Choose fonts and sizes that are easy to read
4. **Performance**: Keep CSS simple and efficient
5. **Compatibility**: Test your theme on different screen sizes

### CSS Tips

1. **Start Simple**: Begin with basic colors and gradually add complexity
2. **Use Variables**: Define colors at the top of your CSS for easy changes
3. **Test Thoroughly**: Check all UI elements with your theme
4. **Document**: Add comments explaining your color choices
5. **Backup**: Keep copies of your working themes

## 🌟 Theme Showcase

### Submit Your Theme

Want to showcase your theme? Create a pull request with:

1. **Your theme files** in `src/qt/themes/custom/yourtheme/`
2. **Preview screenshot** showing your theme in action
3. **Brief description** of your theme's inspiration and features
4. **README section** explaining your design choices

### Community Themes

Check out themes created by the Dogecoin community in the `shared/` directory!

## 📚 Resources

### Learning CSS for Qt

- [Qt Stylesheet Reference](https://doc.qt.io/qt-5/stylesheet-reference.html)
- [Qt CSS Selectors](https://doc.qt.io/qt-5/stylesheet-syntax.html#selector-types)
- [Color Palette Tools](https://coolors.co/)

### Inspiration

- Browse existing themes in `src/qt/themes/`
- Check out popular design systems (Material Design, Bootstrap, etc.)
- Look at other cryptocurrency wallet themes

## 🆘 Getting Help

### Community Support

- **GitHub Issues**: Report bugs or request features
- **Discord**: Join the Dogecoin community for help
- **Reddit**: Share your themes and get feedback

### Contributing

- **Submit Themes**: Create pull requests with your themes
- **Improve Documentation**: Help improve this guide
- **Report Bugs**: Help us fix theme-related issues

---

## 🎉 Happy Theming!

Creating custom themes is a great way to personalize your Dogecoin experience and contribute to the community. Don't be afraid to experiment and share your creations!

**Remember**: The best themes are often the ones that reflect your personality and make using Dogecoin more enjoyable. Have fun and be creative! 🐕✨

---

*This guide is part of the Dogecoin Core project. For the latest updates, visit our [GitHub repository](https://github.com/dogecoin/dogecoin).*
