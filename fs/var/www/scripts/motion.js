var originalMotionInfo;

function preview(img, selection) {
  if (!selection.width || !selection.height) {
    return;
  }

  var image = $(img);

  var naturalWidth = image.prop('naturalWidth');
  var naturalHeight = image.prop('naturalHeight');

  var renderedWidth = image.width();
  var renderedHeight = image.height();

  var x0 = Math.round((selection.x1 / renderedWidth) * naturalWidth);
  var y0 = Math.round((selection.y1 / renderedHeight) * naturalHeight);
  var x1 = Math.round((selection.x2 / renderedWidth) * naturalWidth);
  var y1 = Math.round((selection.y2 / renderedHeight) * naturalHeight);

  $('#x0').val(x0);
  $('#y0').val(y0);
  $('#x1').val(x1);
  $('#y1').val(y1);
  $('#region_xy').html('[ (' + x0 + ', ' + y0 + '), (' + x1 + ', ' + y1 + ') ]');

  if (originalMotionInfo.region_of_interest[0] != x0 ||
    originalMotionInfo.region_of_interest[1] != y0 ||
    originalMotionInfo.region_of_interest[2] != x1 ||
    originalMotionInfo.region_of_interest[3] != y1) {
    // restart only if region has changed
    $('#restart_server').val(1);
  } else {
    // user has set it back to original region
    $('#restart_server').val(0);
  }
}

function setTracking(obj, init) {

  var picture = $('#motion_picture');
  var region_disabled = $('#region_disabled');
  var motion_timeout = $('#motion_timeout');
  var ias = picture.imgAreaSelect({
    instance: true
  });

  if ($(obj).prop("checked")) {
    ias.setOptions({
      hide: true
    });
    picture.css('opacity', 0.5);
    region_disabled.show();
    motion_timeout.prop('readonly', false);
    motion_timeout.css('backgroundColor', '#fff');

  } else {
    ias.setOptions({
      show: true
    });
    picture.css('opacity', 1);
    region_disabled.hide();
    // grey out and disable field
    motion_timeout.prop('readonly', true);
    motion_timeout.css('backgroundColor', '#ccc');
  }
  ias.update();

  if (init == false) {
    $('#restart_server').val(1);
  }

}

function motion_selection_init() {

  // get motion config info
  $.ajax({
    'url': 'cgi-bin/getMotionInfo.cgi',
    'dataType': 'json'
  }).done(function(motionInfo) {

    originalMotionInfo = motionInfo;

    var picture = $('#motion_picture');
    var renderedWidth = picture.width();
    var renderedHeight = picture.height();
    var naturalWidth = picture.prop('naturalWidth');
    var naturalHeight = picture.prop('naturalHeight');

    console.log(renderedWidth);
    console.log(renderedHeight);
    console.log(naturalWidth);
    console.log(naturalHeight);

    var ias = picture.imgAreaSelect({
      instance: true
    });
    ias.setOptions({
      handles: true,
      fadeSpeed: 200,
      movable: true,
      onSelectChange: function(img, selection) {
        preview(img, selection)
      },
      parent: '#motion_img_container'
    });

    var selection = [0, 0, 0, 0];
    if (motionInfo.region_of_interest[0] + motionInfo.region_of_interest[1] +
      motionInfo.region_of_interest[2] + motionInfo.region_of_interest[3] == 0 ||
      (motionInfo.region_of_interest[0] > naturalWidth ||
        motionInfo.region_of_interest[1] > naturalHeight ||
        motionInfo.region_of_interest[2] > naturalWidth ||
        motionInfo.region_of_interest[3] > naturalHeight)
    ) {
      // if no region of interest set (0, 0, 0, 0)
      // or if region of interest set is greater than the current image sizes
      ias.setSelection(0, 0, naturalWidth, naturalHeight, true);
      topLeft = [0, 0];
      selection = [0, 0, naturalWidth, naturalHeight];
      if (naturalWidth > renderedWidth) {
        // scaled image
        selection[2] = renderedWidth;
        selection[3] = renderedHeight;
      }
    } else {
      selection = motionInfo.region_of_interest;
      if (naturalWidth > renderedWidth) {
        // scaled image
        selection = [
          Math.round((motionInfo.region_of_interest[0] / naturalWidth) * renderedWidth),
          Math.round((motionInfo.region_of_interest[1] / naturalHeight) * renderedHeight),
          Math.round((motionInfo.region_of_interest[2] / naturalWidth) * renderedWidth),
          Math.round((motionInfo.region_of_interest[3] / naturalHeight) * renderedHeight),
        ];
      }
    }
    ias.setSelection(selection[0], selection[1], selection[2], selection[3], false);
    ias.setOptions({
      show: true
    });
    ias.update();

    $('#region_xy').html(
      '[ (' + motionInfo.region_of_interest[0] + ', ' + motionInfo.region_of_interest[1] +
      '), (' + motionInfo.region_of_interest[2] + ', ' + motionInfo.region_of_interest[3] + ') ]');

    $('#x0').val(motionInfo.region_of_interest[0]);
    $('#y0').val(motionInfo.region_of_interest[1]);
    $('#x1').val(motionInfo.region_of_interest[2]);
    $('#y1').val(motionInfo.region_of_interest[3]);
    $('#restart_server').val(0);
    $('#motion_indicator_color').val(motionInfo.motion_indicator_color).change();
    $('#motion_sensitivity').val(motionInfo.motion_sensitivity).change();
    $('#motion_tracking').prop('checked', motionInfo.motion_tracking);
    $('#motion_timeout').val(motionInfo.motion_timeout).change();
    //setTracking($('#motion_tracking')[0], true);
  });

  $('#confMotionForm').submit(function(event) {
    var b = $('#configMotionSubmit');
    b.toggleClass('is-loading');
    b.prop('disabled', !b.prop('disabled'));
    var motion_tracking = '';
    if ($('#motion_tracking').prop('checked')) {
      motion_tracking = 'true';
    }
    var formData = {
      'x0': $('input[name=x0]').val(),
      'y0': $('input[name=y0]').val(),
      'x1': $('input[name=x1]').val(),
      'y1': $('input[name=y1]').val(),
      'restart_server': $('input[name=restart_server]').val(),
      'motion_sensitivity': $('select[name=motion_sensitivity]').val(),
      'motion_indicator_color': $('select[name=motion_indicator_color]').val(),
      'motion_tracking': motion_tracking,
      'motion_timeout': $('input[name=motion_timeout]').val()
    };
    $.ajax({
      type: 'POST',
      url: $('#confMotionForm').attr('action'),
      data: formData,
      dataType: 'html',
      encode: true
    }).done(function(res) {
      b.toggleClass('is-loading');
      b.prop('disabled', !b.prop('disabled'));
      showResult(res);
    });
    event.preventDefault();
  });

}

function motion_selection_remove() {
  var picture = $('#motion_picture');
  var ias = picture.imgAreaSelect({
    instance: true
  });
  ias.setOptions({
    remove: true
  });
  ias.update();
}
