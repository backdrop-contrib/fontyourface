(function ($) {

  Backdrop.behaviors.fontsComSettings = {

    attach:function(context, settings) {

      $('#edit-auth a.fieldset-title').click(function() {

        fontsComSettingsCollapse('#edit-pass');
        fontsComSettingsCollapse('#edit-create');

      });

      $('#edit-pass a.fieldset-title').click(function() {

        fontsComSettingsCollapse('#edit-auth');
        fontsComSettingsCollapse('#edit-create');

      });

      $('#edit-create a.fieldset-title').click(function() {

        fontsComSettingsCollapse('#edit-auth');
        fontsComSettingsCollapse('#edit-pass');

      });

    } // attach

  } // Backdrop.behaviors.fontsComSettings

  function fontsComSettingsCollapse(id) {

    if (!$(id).is('.collapsed')) {
      Backdrop.toggleFieldset(id);
    } // if

  } // fontsComSettingsCollapse

})(jQuery);
