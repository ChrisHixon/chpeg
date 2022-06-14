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
grammar.getSession().setMode("ace/mode/yaml");
const code = setupEditorArea("code-editor", "codeText");

const codeAst = setupInfoArea("code-ast");
const codeProfile = setupInfoArea("code-profile");

$('#opt-mode').val(localStorage.getItem('optimazationMode') || '2');
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
  localStorage.setItem('optimazationMode', $('#opt-mode').val());
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

  const optimazationMode = $('#opt-mode').val();
  const packrat = $('#packrat').prop('checked');

  $grammarInfo.html('');
  $grammarValidation.hide();
  $codeInfo.html('');
  $codeValidation.hide();
  codeAst.setValue('');
  codeProfile.setValue('');

  // When grammar is empty we parse the input as a grammar with the default parser
  //if (grammarText.length === 0) return;

  $('#overlay').css({
    'z-index': '1',
    'display': 'block',
    'background-color': 'rgba(0, 0, 0, 0.1)'
  });
  window.setTimeout(() => {
    const data = JSON.parse(Module.lint(grammarText, codeText, optimazationMode ? parseInt(optimazationMode) : 2, packrat));
      $('#overlay').css({
        'z-index': '-1',
        'display': 'none',
        'background-color': 'rgba(1, 1, 1, 1.0)'
      });

    if (data.grammar_valid) {
      $grammarValidation.removeClass('validation-invalid').show();

      if(data.source_valid) {
        codeAst.insert(data.ast);
        codeProfile.insert(data.profile);
      }

      if (data.source_valid) {
        $codeValidation.removeClass('validation-invalid').show();
      } else {
        $codeValidation.addClass('validation-invalid').show();
      }

      if (data.code.length > 0) {
        const html = generateErrorListHTML(data.code);
        $codeInfo.html(html);
      }
    } else {
      $grammarValidation.addClass('validation-invalid').show();
    }

    if (data.grammar.length > 0) {
      const html = generateErrorListHTML(data.grammar);
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
  }, 750);
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

// Event handing in the AST optimazation
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
	codeAst.resize();
  codeAst.renderer.updateFull();
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
setupToolWindow('show-ast', '#show-ast', '#code-ast');
setupToolWindow('show-profile', '#show-profile', '#code-profile');

// Show page
$('#main').css({
  'display': 'flex',
});

// WebAssembly
var Module = {
  onRuntimeInitialized: function() {
    // Initial parse
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  }
};
