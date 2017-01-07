File Force Download
-------------------

Originally provided by Garrett Albright
Maintained by Martin B. - martin@webscio.net
Supported by JoyGroup - http://www.joygroup.nl


Installation
------------
 * Extract the module archive, copy its contents to your modules directory and activate the module.


Usage - Automatic
-----------------
 * File Force Download offers a number of additional formatters to the supported fields, which can be selected
 either in the "Display Fields" section of your content type or when adding fields to your view.


Usage - Manual
--------------
 * With the "Private" download method enabled on your site in admin/settings/file-system: you can simply use the
 url() & l() functions with an additional item in the options array, e.g. url($image['filepath'], array('query'
 => array('download' => '1'))).

 * With the "Public" download method enabled by default, you will need to make sure that the File Force
 link is going through Drupal by passing it through file_force_create_url() first, e.g. l('linktext',
 file_force_create_url($image['filepath'], FALSE), array('query' => array('download' => '1'))).
 
 * The file_force_create_url method will return an URL with the 'download=1' query appended to it by default.
 This behaviour can be altered by setting its second parameter to FALSE.
 
 * Setting 'download' to '1' will result in download being forced, while setting it to '0' will force the item to 
 be displayed inline.


Supported Modules
-----------------
 * File
 * Image


For more instructions, please see this page in the Drupal manual:
http://drupal.org/node/265424
