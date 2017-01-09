(function($) {
  /**
   * @file
   * JS support for favorites.
   */

  /**
   * Create variables and functions.
   */
  Drupal.favorites = {
    /**
     * Reads the form data and passes it to the Ajax callback.
     * Drupal's "ahah" extensions would make it too complicated
     * dynamically processing the form AND rebuilding the
     * favorites list.
     */
    add: function() {
      jQuery.ajax({
        url: Drupal.settings.favorites.addCallbackPath,
        dataType: 'json',
        type: 'POST',
        data: Drupal.favorites.extractFormData(),
        error: function(a, b, c) {
          alert('An error occured.\n\nStatus:\n' + b + '\n\nMessage:\n' + c);
        },
        success: function(data) {
          Drupal.favorites.rebuild(data);

          // Collapse the form fieldset again.
          var fs = $('form#favorites-add-favorite-form fieldset#edit-add');
          if (!fs.is('.collapsed')) {
            fs.find("a.fieldset-title").first().click();
          }
         // Call draggable method after add favorite.
          Drupal.behaviors.favoritesLinks.applyDraggable();
        }
      });
    },
    extractFormData: function() {
      return {
        path: Drupal.settings.favorites.path,
        query: Drupal.settings.favorites.query,
        title: $('form#favorites-add-favorite-form input[name=title]').val()
      };
    },
    rebuild: function(data) {
      var list = $('div#favorites-list');
      Drupal.detachBehaviors(list);
      list.html(data.list);
      Drupal.attachBehaviors(list);
    },
    remove: function(caller) {
      jQuery.ajax({
        url: caller.attr('href').replace(/\?.*$/,''),
        dataType: 'json',
        data: caller.attr('href').replace(/^.*?\?/,'') + '&js=1',
        error: function(a, b, c) {
          alert('An error occured.\n\nStatus:\n' + b + '\n\nMessage:\n' + c);
        },
        success: function(data) {
          Drupal.favorites.rebuild(data);
        }
      });
    }
  }

  /**
   * Init dynamic link extensions.
   */
  Drupal.behaviors.favoritesLinks = {
    attach: function(context) {
      $('a.favorites-remove:not(.js-processed)',context).click(function(event){
        Drupal.favorites.remove($(this));
        event.preventDefault();
      }).addClass('js-processed');
      // We need to suppress any native submit options for the form before we add
      $('form#favorites-add-favorite-form:not(.js-processed)', context).submit(function(event){
        Drupal.favorites.add();
        event.preventDefault();
      }).addClass('js-processed');
      this.applyDragHandler();
      this.applyDraggable();
    },
    // Add draggable handle.
    applyDragHandler: function() {
      $('.collapse-block').remove();
      var found_toggle = $('.block-favorites .collapse-block').length;
      if (found_toggle == 0) {
        $('.block-favorites h2').prepend('<span class="collapse-block collapse-true" style="color: rgb(204, 204, 204); ' +
          'font-size: 15px; cursor: pointer; margin-left: -17px; margin-right: 5px;">-</span>');
      }
      $(".block-favorites h2 .collapse-block").toggle(
        function() {
          $(".block-favorites #favorites-list").toggle();
          $(this).removeClass("collapse-true");
          $(this).addClass("collapse-false");
          $('.collapse-block').css({'font-size': '10px'}).html('+');
        },
        function() {
          $(".block-favorites #favorites-list").toggle();
          $(this).removeClass("collapse-false");
          $(this).addClass("collapse-true");
          $('.collapse-block').css({'font-size': '15px'}).html('-');
        }
      );
      var found_handles = $('#favorites-list .handle').length;
      if (found_handles == 0) {
        $("#favorites-list ul").children().each(function(i) {
          var handlemarkup = '<a class="tabledrag-handle" href="#" title="Drag to re-order"><span class="handle" ' +
            'style="padding-left: 10px; opacity: 0; cursor: move;">&nbsp;</span></a>';
          $(this).prepend(handlemarkup);
          $('.hidden-fid').hide();
          console.log($(this).parent('ul'));
          $(this).parent('ul').css({
            'padding': "0px",
          });        
          $(this).css({
            'list-style': "disc",
            'list-style-position': "inside",
            'text-indent': '-13px',
            'padding-left': '13px',
          });
        });
      }
    },
    // Add dragable feature.
    applyDraggable: function() {
      // Make collapsable favorites block.
      $('#favorites-list li').find(".tabledrag-handle .handle").css({
        'visibility': "hidden",
        'height': '0px',
        'margin': '0px',
        'padding': '0px',
        'width': '0px',
      });
      // Control handler visibility on hover.
      $('#favorites-list li').hover(
        function() {
          $(this).find(".tabledrag-handle .handle").css({
            'visibility': "visible",
            'height': '13px',
            'margin': '0px 5px 0px -21px',
            'padding': '8px 16px 0 0',
            'width': '13px',
          });
        }, function() {
        $(this).find(".tabledrag-handle .handle").css({
          'visibility': "hidden",
          'height': '0px',
          'margin': '0px',
          'padding': '0px',
          'width': '0px',
        });
      });
      // Add custom sorting.
      $('#favorites-list ul').sortable({
        containment: '.block-favorites',
        axis: 'y',
        update: function(event, ui) {
        var stringDiv = "";
        $("#favorites-list ul").children().each(function(i) {
          var li = $(this);
          var fav_id = li.children('.favorites-remove').attr('fid');
          var fav_weight = i + 1;
          $.ajax({
            type: 'GET',
            url: Drupal.settings.basePath + 'favorites/js/order/' + fav_id + '/' + fav_weight,
          });  
        });
        }
      });
    }
  }
})(jQuery);
