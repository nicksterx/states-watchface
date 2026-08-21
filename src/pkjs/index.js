/*
 * States Watchface — phone-side settings
 *
 * Renders the settings page as an inline data: URI (no hosting needed)
 * and sends the chosen settings to the watch over AppMessage.
 */

var STORAGE_KEY = 'settings';

// Pebble-safe hex colors (multiples of 0x55 render exactly on color watches).
var BG_COLORS   = ['000000', 'FFFFFF', '000055'];
var FACT_COLORS = ['FFFF00', 'FFAA00', 'FF0000', '00FF00',
                   '00FFFF', 'FF00FF', 'FFFFFF', '0000FF'];

var DEFAULTS = {
  showSubtitle: true,
  backgroundColor: '000000',
  factColor: 'FFFF00'
};

// The watch defaults to Large on emery (Pebble Time 2) — mirror that here so
// the page opens showing what the watch is actually displaying.
function defaultTextSize() {
  try {
    var info = Pebble.getActiveWatchInfo && Pebble.getActiveWatchInfo();
    if (info && info.platform === 'emery') {
      return 1;
    }
  } catch (err) { /* fall through */ }
  return 0;
}

function clampTextSize(size) {
  size = parseInt(size, 10);
  return (size >= 0 && size <= 2) ? size : defaultTextSize();
}

function validHex(hex, fallback) {
  return /^[0-9A-Fa-f]{6}$/.test(String(hex)) ? String(hex).toUpperCase()
                                              : fallback;
}

function loadSettings() {
  var s = {};
  try {
    s = JSON.parse(localStorage.getItem(STORAGE_KEY)) || {};
  } catch (err) {
    s = {};
  }
  return {
    textSize: clampTextSize(s.textSize !== undefined
        ? s.textSize
        : localStorage.getItem('textSize')),  // migrate pre-1.3 key
    showSubtitle: s.showSubtitle !== false,
    backgroundColor: validHex(s.backgroundColor, DEFAULTS.backgroundColor),
    factColor: validHex(s.factColor, DEFAULTS.factColor)
  };
}

function swatchRow(group, colors, selected) {
  var html = '<div class="swatches">';
  for (var i = 0; i < colors.length; i++) {
    var c = colors[i];
    html += '<span class="sw' + (c === selected ? ' on' : '') +
            '" style="background:#' + c + '" ' +
            'onclick="pick(\'' + group + '\',\'' + c + '\',this)"></span>';
  }
  return html + '</div>';
}

function buildConfigPage(s) {
  var html =
    '<!DOCTYPE html><html><head>' +
    '<meta charset="utf-8">' +
    '<meta name="viewport" content="width=device-width, initial-scale=1">' +
    '<title>50 States Settings</title>' +
    '<style>' +
    'body{font-family:-apple-system,Helvetica,Arial,sans-serif;background:#333;color:#fff;margin:0;padding:20px}' +
    'h1{font-size:20px;margin:0 0 16px}' +
    'h2{font-size:15px;color:#ccc;margin:20px 0 8px}' +
    'p.hint{color:#aaa;font-size:13px;margin:4px 0 0}' +
    'label{display:block;background:#444;border-radius:6px;padding:14px;margin-bottom:8px;font-size:16px}' +
    'input{margin-right:10px}' +
    '.swatches{line-height:0}' +
    '.sw{width:40px;height:40px;border-radius:50%;display:inline-block;border:3px solid #666;margin:0 10px 10px 0}' +
    '.sw.on{border-color:#fff;box-shadow:0 0 0 2px #ff4700}' +
    'button{width:100%;padding:14px;font-size:17px;border:0;border-radius:6px;background:#ff4700;color:#fff;margin-top:24px}' +
    '</style></head><body>' +
    '<h1>50 States Settings</h1>' +

    '<h2>Text Size</h2>' +
    '<label><input type="radio" name="size" value="0"' + (s.textSize === 0 ? ' checked' : '') + '>Normal</label>' +
    '<label><input type="radio" name="size" value="1"' + (s.textSize === 1 ? ' checked' : '') + '>Large</label>' +
    '<label><input type="radio" name="size" value="2"' + (s.textSize === 2 ? ' checked' : '') + '>Extra Large</label>' +
    '<p class="hint">Pebble Time 2 defaults to Large — its sharper screen makes text look smaller.</p>' +

    '<h2>Details</h2>' +
    '<label><input type="checkbox" id="sub"' + (s.showSubtitle ? ' checked' : '') + '>Show capital &amp; year line</label>' +
    '<p class="hint">Hiding it frees room for the fact text — handy with Extra Large.</p>' +

    '<h2>Background Color</h2>' +
    swatchRow('bg', BG_COLORS, s.backgroundColor) +

    '<h2>Fact Color</h2>' +
    swatchRow('fact', FACT_COLORS, s.factColor) +
    '<p class="hint">Colors apply on color screens; black &amp; white watches pick the closest shade.</p>' +

    '<button onclick="save()">Save</button>' +
    '<script>' +
    'var sel={bg:"' + s.backgroundColor + '",fact:"' + s.factColor + '"};' +
    'function pick(group,hex,el){' +
    'sel[group]=hex;' +
    'var p=el.parentNode.children;' +
    'for(var i=0;i<p.length;i++){p[i].className="sw";}' +
    'el.className="sw on";' +
    '}' +
    'function save(){' +
    'var size=document.querySelector(\'input[name="size"]:checked\').value;' +
    'var out={textSize:parseInt(size,10),' +
    'showSubtitle:document.getElementById("sub").checked,' +
    'backgroundColor:sel.bg,factColor:sel.fact};' +
    'location.href="pebblejs://close#"+encodeURIComponent(JSON.stringify(out));' +
    '}' +
    '<\/script>' +
    '</body></html>';
  return 'data:text/html;charset=utf-8,' + encodeURIComponent(html);
}

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL(buildConfigPage(loadSettings()));
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) {
    return;  // user backed out without saving
  }
  var raw;
  try {
    raw = JSON.parse(decodeURIComponent(e.response));
  } catch (err) {
    return;
  }

  var settings = {
    textSize: clampTextSize(raw.textSize),
    showSubtitle: !!raw.showSubtitle,
    backgroundColor: validHex(raw.backgroundColor, DEFAULTS.backgroundColor),
    factColor: validHex(raw.factColor, DEFAULTS.factColor)
  };
  localStorage.setItem(STORAGE_KEY, JSON.stringify(settings));

  Pebble.sendAppMessage({
    TextSize: settings.textSize,
    ShowSubtitle: settings.showSubtitle ? 1 : 0,
    BackgroundColor: parseInt(settings.backgroundColor, 16),
    FactColor: parseInt(settings.factColor, 16)
  });
});
