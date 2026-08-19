/*
 * States Watchface — phone-side settings
 *
 * Renders the settings page as an inline data: URI (no hosting needed)
 * and sends the chosen text size to the watch over AppMessage.
 */

var STORAGE_KEY = 'textSize';

function clampTextSize(size) {
  size = parseInt(size, 10);
  return (size >= 0 && size <= 2) ? size : 0;
}

function getTextSize() {
  return clampTextSize(localStorage.getItem(STORAGE_KEY));
}

function buildConfigPage(currentSize) {
  var html =
    '<!DOCTYPE html><html><head>' +
    '<meta charset="utf-8">' +
    '<meta name="viewport" content="width=device-width, initial-scale=1">' +
    '<title>50 States Settings</title>' +
    '<style>' +
    'body{font-family:-apple-system,Helvetica,Arial,sans-serif;background:#333;color:#fff;margin:0;padding:20px}' +
    'h1{font-size:20px;margin:0 0 4px}' +
    'p.hint{color:#aaa;font-size:13px;margin:0 0 20px}' +
    'label{display:block;background:#444;border-radius:6px;padding:14px;margin-bottom:10px;font-size:16px}' +
    'input{margin-right:10px}' +
    'button{width:100%;padding:14px;font-size:17px;border:0;border-radius:6px;background:#ff4700;color:#fff;margin-top:10px}' +
    '</style></head><body>' +
    '<h1>Text Size</h1>' +
    '<p class="hint">Pebble Time 2 defaults to Large — its sharper screen makes text look smaller. Try Extra Large for maximum readability.</p>' +
    '<label><input type="radio" name="size" value="0"' + (currentSize === 0 ? ' checked' : '') + '>Normal</label>' +
    '<label><input type="radio" name="size" value="1"' + (currentSize === 1 ? ' checked' : '') + '>Large</label>' +
    '<label><input type="radio" name="size" value="2"' + (currentSize === 2 ? ' checked' : '') + '>Extra Large</label>' +
    '<button onclick="save()">Save</button>' +
    '<script>' +
    'function save(){' +
    'var size=document.querySelector(\'input[name="size"]:checked\').value;' +
    'location.href="pebblejs://close#"+encodeURIComponent(JSON.stringify({textSize:parseInt(size,10)}));' +
    '}' +
    '<\/script>' +
    '</body></html>';
  return 'data:text/html;charset=utf-8,' + encodeURIComponent(html);
}

Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL(buildConfigPage(getTextSize()));
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (!e || !e.response) {
    return;  // user backed out without saving
  }
  var settings;
  try {
    settings = JSON.parse(decodeURIComponent(e.response));
  } catch (err) {
    return;
  }
  var size = clampTextSize(settings.textSize);
  localStorage.setItem(STORAGE_KEY, String(size));
  Pebble.sendAppMessage({ TextSize: size });
});
