(function ($) {

  Backdrop.behaviors.fontsComProject = {

    attach:function(context, settings) {

      $('#edit-edit-project').hide();

      if ($('#edit-project').val() != '') {
        $('#edit-project').change();
      }

    }

  }

})(jQuery);
