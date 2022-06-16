// vim: sw=2:sts=2

// Setup editros
function setupInfoArea(id) {
  const e = ace.edit(id);
  e.setShowPrintMargin(false);
  e.setOptions({
    readOnly: true,
    highlightActiveLine: false,
    highlightGutterLine: false
  })
  e.renderer.$cursorLayer.element.style.opacity=0;
  return e;
}

function setupEditorArea(id, lsKey) {
  const e = ace.edit(id);
  e.setShowPrintMargin(false);
  e.setValue(localStorage.getItem(lsKey) || '');
  e.moveCursorTo(0, 0);
  return e;
}

const grammar = setupEditorArea("grammar-editor", "grammarText");
const code = setupEditorArea("code-editor", "codeText");

//const codeAst = setupInfoArea("code-ast");
const codeAstOptimized = setupInfoArea("code-ast-optimized");
const codeProfile = setupInfoArea("code-profile");

$('#opt-mode').val(localStorage.getItem('optimizationMode') || '2');
$('#packrat').prop('checked', localStorage.getItem('packrat') === 'true');
$('#auto-refresh').prop('checked', localStorage.getItem('autoRefresh') === 'true');
$('#parse').prop('disabled', $('#auto-refresh').prop('checked'));

// Parse
function escapeHtml(unsafe) {
  return unsafe
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

function nl2br(str) {
  return str.replace(/\n/g, '<br>\n')
}

function generateErrorListHTML(errors) {
  let html = '<ul>';

  html += $.map(errors, function (x) {
    return '<li data-ln="' + x.ln + '" data-col="' + x.col + '"><span>' + x.ln + ':' + x.col + '</span> <span>' + escapeHtml(x.msg) + '</span></li>';
  }).join('');

  html += '<ul>';

  return html;
}

function updateLocalStorage() {
  localStorage.setItem('grammarText', grammar.getValue());
  localStorage.setItem('codeText', code.getValue());
  localStorage.setItem('optimizationMode', $('#opt-mode').val());
  localStorage.setItem('packrat', $('#packrat').prop('checked'));
  localStorage.setItem('autoRefresh', $('#auto-refresh').prop('checked'));
}

function parse() {
  const $grammarValidation = $('#grammar-validation');
  const $grammarInfo = $('#grammar-info');
  const grammarText = grammar.getValue();

  const $codeValidation = $('#code-validation');
  const $codeInfo = $('#code-info');
  const codeText = code.getValue();

  const optimizationMode = $('#opt-mode').val();
  const packrat = $('#packrat').prop('checked');

  $grammarInfo.html('');
  $grammarValidation.hide();
  $codeInfo.html('');
  $codeValidation.hide();
  //codeAst.setValue('');
  codeAstOptimized.setValue('');
  codeProfile.setValue('');

  outputs.compile_status = '';
  outputs.parse_status = '';
  outputs.ast = '';

  if (grammarText.length === 0) {
    return;
  }

  $('#overlay').css({
    'z-index': '1',
    'display': 'block',
    'background-color': 'rgba(0, 0, 0, 0.1)'
  });
  window.setTimeout(() => {
    //const data = JSON.parse(Module.lint(grammarText, codeText, mode, packrat));
    chpeg_parse(grammarText, codeText, optimizationMode, packrat);

    $('#overlay').css({
      'z-index': '-1',
      'display': 'none',
      'background-color': 'rgba(1, 1, 1, 1.0)'
    });

    if (result.compile == 0) {
      $grammarValidation.removeClass('validation-invalid').show();

      //codeAst.insert('');
      codeAstOptimized.insert(outputs.ast);
      codeProfile.insert('');

      if (result.parse == 0) {
        $codeValidation.removeClass('validation-invalid').show();
      } else {
        $codeValidation.addClass('validation-invalid').show();
      }

      if (outputs.parse_status.length > 0) {
        const html = nl2br(escapeHtml(outputs.parse_status));
        $codeInfo.html(html);
      }
    } else {
      $grammarValidation.addClass('validation-invalid').show();
    }

    if (outputs.compile_status.length > 0) {
      const html = nl2br(escapeHtml(outputs.compile_status));
      $grammarInfo.html(html);
    }
  }, 0);
}

// Event handing for text editiing
let timer;
function setupTimer() {
  clearTimeout(timer);
  timer = setTimeout(() => {
    updateLocalStorage();
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  }, 100);
};
grammar.getSession().on('change', setupTimer);
code.getSession().on('change', setupTimer);

// Event handing in the info area
function makeOnClickInInfo(editor) {
  return function () {
    const el = $(this);
    editor.navigateTo(el.data('ln') - 1, el.data('col') - 1);
    editor.scrollToLine(el.data('ln') - 1, true, false, null);
    editor.focus();
  }
};
$('#grammar-info').on('click', 'li', makeOnClickInInfo(grammar));
$('#code-info').on('click', 'li', makeOnClickInInfo(code));

// Event handing in the AST optimization
$('#opt-mode').on('change', setupTimer);
$('#packrat').on('change', setupTimer);
$('#auto-refresh').on('change', () => {
  updateLocalStorage();
  $('#parse').prop('disabled', $('#auto-refresh').prop('checked'));
  setupTimer();
});
$('#parse').on('click', parse);

// Resize editors to fit their parents
function resizeEditorsToParent() {
  code.resize();
  code.renderer.updateFull();
  //codeAst.resize();
  //codeAst.renderer.updateFull();
  codeAstOptimized.resize();
  codeAstOptimized.renderer.updateFull();
  codeProfile.resize();
  codeProfile.renderer.updateFull();
}

// Show windows
function setupToolWindow(lsKeyName, buttonSel, codeSel) {
  let show = localStorage.getItem(lsKeyName) === 'true';
  $(buttonSel).prop('checked', show);
  $(codeSel).css({ 'display': show ? 'block' : 'none' });

  $(buttonSel).on('change', () => {
    show = !show;
    localStorage.setItem(lsKeyName, show);
    $(codeSel).css({ 'display': show ? 'block' : 'none' });
    resizeEditorsToParent();
  });
}
//setupToolWindow('show-ast', '#show-ast', '#code-ast');
setupToolWindow('show-ast-optimized', '#show-ast-optimized', '#code-ast-optimized');
setupToolWindow('show-profile', '#show-profile', '#code-profile');

// Show page
$('#main').css({
  'display': 'flex',
});

// used to collect output from C
var outputs = {
  'default': '',
  'compile_status': '',
  'parse_status': '',
  'ast': '',
};

// current output (key in `outputs`)
var output = "default";

// results of the various stages
var result = {
  'compile': 0,
  'parse': 0,
  'ast': 0,
};

// chpeg_parse function: initialized when emscripten runtime loads
var chpeg_parse = null;

// Emscripten
var Module = {

  // intercept stdout (print) and stderr (printErr)
  // note: text received is line based and missing final '\n'

  'print': function(text) {
    outputs[output] += text + "\n";
  },
  'printErr': function(text) {
    outputs[output] += text + "\n";
  },

  // called when emscripten runtime is initialized
  'onRuntimeInitialized': function() {
    // wrap the C `parse` function
    chpeg_parse = cwrap('parse', 'number', ['string', 'string', 'number']);
    // Initial parse
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  },
};

