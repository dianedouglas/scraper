<?php
/**
 * Implementation of hook_audiofield_players().
 */
function audiofield_players() {
  $players = module_invoke_all('audiofield_players');

  $players['html5'] = array(
    'name' => 'HTML5 <audio>',
    'filetypes' => array('mp3', 'wav', 'ogg'),
    'local' => TRUE,
    'callback' => 'audiofield_html5_audio',
  );

  $players['wpaudioplayer'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/audio-player/player.swf",
    'name' => 'WordPress Audio Player',
    'download_link' => 'http://wpaudioplayer.com/download',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_wpaudioplayer',
  );

  $players['xspf_slim'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/xspf_player_slim.swf",
    'name' => 'XSPF Slim Player',
    'download_link' => 'http://prdownloads.sourceforge.net/musicplayer/xspf_player_slim-correct-0.2.3.zip?download',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_xspf_slim',
  );

  $players['xspf_button'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/button/musicplayer.swf",
    'name' => 'XSPF Button Player',
    'download_link' => 'http://prdownloads.sourceforge.net/musicplayer/button_player-0.1.zip?download',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_xspf_button',
  );

  //#1414398: Premium Beat players no longer available
  $players['premium_beat_single_track'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/playerSinglePackage/playerSingle.swf",
    'name' => 'Premium Beat Single Player',
    //'download_link' => 'http://www.premiumbeat.com/flash_resources/free_flash_music_player/single_track_flash_mp3_player.php',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_premium_beat_single_track',
  );

  $players['premium_beat_thin'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/OriginalThinMusicPlayer.swf",
    'name' => 'Premium Beat Thin Player',
    //'download_link' => 'http://www.premiumbeat.com/flash_music_players/original/thin/',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_premium_beat_thin',
  );

  $players['premium_beat_mini'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/LWMusicPlayer.swf",
    'name' => 'Premium Beat Mini Player',
    //'download_link' => 'http://www.premiumbeat.com/flash_music_players/original/mini/',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_premium_beat_mini',
  );

    $players['flowplayer'] = array(
      'name' => 'Flowplayer',
      'download_link' => 'http://drupal.org/project/flowplayer',
      'filetypes' => array('mp3'),
      'callback' => 'audiofield_flowplayer',
      'module' => 'flowplayer',
    );


  $players['soundmanager2'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2",
    'name' => 'SoundManager2 360',
    'download_link' => 'http://www.schillmania.com/projects/soundmanager2/',
    'filetypes' => array('mp3', 'wav'),
    'callback' => 'audiofield_soundmanager2',
  );

    $players['jplayer'] = array(
      'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/jplayer/jquery.jplayer.min.js",
      'css' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/jplayer/jplayer.css",
      'name' => 'jPlayer',
      'download_link' => 'https://www.drupal.org/project/jplayer',
      'module' => 'jplayer',
      'filetypes' => array('mp3'),
      'callback' => 'audiofield_jplayer',
    );

  return $players;
}

function _audiofield_theme() {
  return array(
    // Themes for the players.
    'audiofield_players_wpaudioplayer' => array(
      'variables' => array('player_path' => NULL, 'audio_file' => NULL),
    ),
    'audiofield_players_xspf_slim' => array(
      'variables' => array('player_path' => NULL, 'audio_file' => NULL),
    ),
    'audiofield_players_xspf_button' => array(
      'variables' => array('player_path' => NULL, 'audio_file' => NULL),
    ),
    'audiofield_players_premium_beat_single_track' => array(
      'variables' => array('player_path' => NULL, 'audio_file' => NULL),
    ),
    'audiofield_players_premium_beat_thin' => array(
      'variables' => array('player_path' => NULL, 'audio_file' => NULL),
    ),
    'audiofield_players_premium_beat_mini' => array(
      'variables' => array('player_path' => NULL, 'audio_file' => NULL),
    ),
  );
}

/* Audio Callback Functions */

function audiofield_embeddedplayer($audio_file) {
  return '<audio src="' . $audio_file . '" controls><embed height="15" src="' . $audio_file . '" autostart="false"></embed></audio>';
}

function audiofield_html5_audio($player_path, $audio_file) {
  return '<audio src="' . $audio_file . '" controls></audio>';
}

function audiofield_wpaudioplayer($player_path, $audio_file) {
  $variables = array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  );
  return theme('audiofield_players_wpaudioplayer', $variables);
}

function theme_audiofield_players_wpaudioplayer($variables) {
  $player_path = $variables['player_path'];
  $audio_file = $variables['audio_file'];
  return '<object class="audioplayer2" height="24" width="290" data="' . $player_path . '" type="application/x-shockwave-flash">
                          <param value="' . $player_path . '" name="movie"/>
                          <param value="playerID=2&amp;bg=0xCDDFF3&amp;leftbg=0x357DCE&amp;lefticon=0xF2F2F2&amp;rightbg=0xF06A51&amp;rightbghover=0xAF2910&amp;righticon=0xF2F2F2&amp;righticonhover=0xFFFFFF&amp;text=0x357DCE&amp;slider=0x357DCE&amp;track=0xFFFFFF&amp;border=0xFFFFFF&amp;loader=0xAF2910&amp;soundFile=' . $audio_file . '" name="FlashVars"/>
                          <param value="high" name="quality"/>
                          <param value="false" name="menu"/>
                          <param value="transparent" name="wmode"/>
                          </object>';
}

function audiofield_xspf_slim($player_path, $audio_file) {
  $variables = array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  );
  return theme('audiofield_players_xspf_slim', $variables);
}

function theme_audiofield_players_xspf_slim($variables) {
  $player_path = $variables['player_path'];
  $audio_file = $variables['audio_file'];
  return '<object type="application/x-shockwave-flash" width="175" height="14"
          data="' . $player_path . '?song_url=' . $audio_file . '">
          <param name="movie" value="' . $player_path . '?song_url=' . $audio_file . '" />
          <param name="wmode" value="transparent" />
          </object>';
}

function audiofield_xspf_button($player_path, $audio_file) {
  $variables = array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  );
  return theme('audiofield_players_xspf_button', $variables);
}

function theme_audiofield_players_xspf_button($variables) {
  $player_path = $variables['player_path'];
  $audio_file = $variables['audio_file'];
  return '<object type="application/x-shockwave-flash" width="17" height="17"
          data="' . $player_path . '?song_url=' . $audio_file . '">
          <param name="movie" value="' . $player_path . '?song_url=' . $audio_file . '" />
          <param name="wmode" value="transparent" />
          </object>';
}

function audiofield_premium_beat_single_track($player_path, $audio_file) {
  $variables = array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  );
  return theme('audiofield_players_premium_beat_single_track', $variables);
}

function theme_audiofield_players_premium_beat_single_track($variables) {
  $player_path = $variables['player_path'];
  $audio_file = $variables['audio_file'];
  return '<object><param name="autoplay" value="true" />
          <param name="controller"value="true" />
          <embed src="' . $player_path . '"  width="192" height="80" float="left" wmode="transparent" flashvars="mediaPath=' . $audio_file . '"
		  autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
          </embed></object>';
}

function audiofield_premium_beat_thin($player_path, $audio_file) {
  $variables = array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  );
  return theme('audiofield_players_premium_beat_thin', $variables);
}

function theme_audiofield_players_premium_beat_thin($variables) {
  $player_path = $variables['player_path'];
  $audio_file = $variables['audio_file'];
  return '<object><param name="autoplay" value="true" />
        <param name="controller"value="true" />
        <embed src="' . $player_path . '"  width="220" height="21" float="left" wmode="transparent" flashvars="mediaPath=' . $audio_file . '&defaultVolume=100" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
        </embed></object>';
}

function audiofield_premium_beat_mini($player_path, $audio_file) {
  $variables = array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  );
  return theme('audiofield_players_premium_beat_mini', $variables);
}

function theme_audiofield_players_premium_beat_mini($variables) {
  $player_path = $variables['player_path'];
  $audio_file = $variables['audio_file'];
  return '<object><param name="autoplay" value="true" />
        <param name="controller"value="true" />
        <embed src="' . $player_path . '"  width="65" height="21" float="left" wmode="transparent" flashvars="mediaPath=' . $audio_file . '&defaultVolume=100" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
        </embed></object>';
}

function audiofield_soundmanager2($player_path, $audio_file) {
  static $instanceIndex = 0;
  if ($audio_file == '') {
    return t('HTML5 media player for soundmanager2 (skin 360-player)');
  }
  else {
    $instanceIndex += 1;
    $player_base_path = variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2";
    $ie_js = array(
      '#type' => 'markup',
      '#markup' => '<!--[if IE]><script type="text/javascript" src="' . $player_path . '/demo/360-player/script/excanvas.js"></script><![endif]-->',
    );
    drupal_add_html_head($ie_js, 'audiofield');

    drupal_add_js($player_base_path . '/script/soundmanager2.js', array('weight' => 0));

    //360 viewer
    drupal_add_js($player_base_path . '/demo/360-player/script/berniecode-animator.js', array('weight' => 1));
    drupal_add_js($player_base_path . '/demo/360-player/script/360player.js', array('weight' => 1));
    drupal_add_css($player_base_path . '/demo/360-player/360player.css');

    return '
    	<div id="sm2-container-real" class="ui360">
		<a href="' . $audio_file . '"></a>
    	</div>
    <script type="text/javascript">

    soundManager.url = "' . $player_path . '/swf"; // directory where SM2 .SWFs live
    soundManager.useFastPolling = true; // increased JS callback frequency, combined with useHighPerformance = true
    soundManager.waitForWindowLoad = true;

    //so Firefox and others without MP3 support work
    soundManager.preferFlash = true;

    // disable debug mode after development/testing..
    //soundManager.debugMode = true;
    soundManager.onready(function () {
      soundManager.stopAll();
      threeSixtyPlayer.init();
    });
    </script>';
  }
}

if (module_exists('flowplayer')) {
  function audiofield_flowplayer($player_path, $audio_file) {
    static $seq = 1;
    $variables = array(
      'config' => array(
        'clip' => array(
          'url' => $audio_file,
          'autoPlay' => FALSE,
        ),
      ),
      'id' => 'audiofield-' . $seq++,
      'attributes' => array('style' => 'height: 24px'),
    );
    return theme('flowplayer', $variables);
  }
}

function audiofield_jplayer($player_path, $audio_file, $options) {
  return theme(
    'jplayer',
    array(
      'entity_type' => $options['entity_type'],
      'entity' => $options['entity'],
      'field_name' => $options['instance']['field_name'],
      'items' => $options['items'],
      'settings' => array(
    'autoplay' => 0,
    'solution' => 'html, flash',
    'preload' => 'metadata',
    'volume' => 80,
    'muted' => FALSE,
    'repeat' => 'none',
    'backgroundColor' => '000000',
    'mode' => 'playlist',
  ),
    )
  );
}
