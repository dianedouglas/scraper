# _My Web Scraper Module_

#### _Export all the HTML from a music review site as a CSV file_

#### By Diane Douglas

## Description

#### [Aquarius Records Memorial](http://live-preserveaq.pantheonsite.io/)

This module was used to read in all 49,424 music reviews from a music store website on webarchive.org. 

* It reads the HTML page by page.
* Formats and stores the data from each review in a multidimensional array.
* Then it exports all reviews in a CSV file so that it can be imported into a Drupal site using the Feeds module.
* The data is formatted to match the fields of a custom content type called a "Review".  

Aquarius Records was a famous independant record store in San Francisco, founded in 1970. It was famous for promoting underground music and their site had thoughtful, eloquent reviews that were fun to browse to discover new music. By using the keyword search on their website you could discover tons of new music by searching not just by album title or artist name, but by the keywords used in reviews such as "psychedelic", "drone", "vintage", "blues", or even names of other bands you like. 

##### Problem:

When the store was sold to new owners, the website was taken down. The archived version does not have a working search function anymore. So I wanted to preserve all that hard work and add some extra features by porting the reviews to a Drupal site. 

##### Solution: 

The Drupal site created with this module could potentially help the former owner if he wants to publish his work as a book, and it could help the community of underground music lovers to continue to grow and find new music. I have also added a "Favorites List" so that a user can bookmark their favorite albums or a page of search results that they want to read and share later. I also added a Forum so that users can share recommendations or share their own music. 

## Setup/Installation Requirements

To load this site:

1. Clone this repository. Launch your server in the top level of the cloned directory.
2. Import the included database into one called 'scrapeaq'. The file is: `sites/db-backup/scrapeaq_mostrecent.sql.zip`.
3. Create a database user with global permissions to access the database. Username and password should be 'scrapeaq'.
4. Navigate your browser to localhost. For example, using MAMP the default address is localhost:8888.

To set up the Feeds modules on your site, you'll need to:

1. Download and enable the following modules and their dependencies: feeds, feeds_ui, feeds_tamper, feeds_tamper_ui
2. Add feed importer by clicking "Configure" next to the Feeds module in the modules admin screen. I kept all defaults except for the following: 

* For basic settings: standalone form, periodic off, import on submission on.
* fetcher: file upload.
* parser: csv
* csv parser settings: default delimiter |, "no headers" unchecked, utf8.
* processor: node
* node processor settings: bundle = 'your content type', "Update existing nodes", "delete non existant nodes",
do not force authorization and nodes don't need expire.
* mapping: This is where you define what the fields in your CSV correspond to which fields in your content type. 

key - Title (title) 
Used as unique.
artist - Artist (field_artist)   
title - Album Title (field_album_title)   
labelinfo - Label Info (field_label_info)   
img - Album Art: URL (field_album_art:url)    
audio - Old Audio Links (Not active, for records keeping purposes): URL (field_audio_links:url)   
audio-title - Old Audio Links (Not active, for records keeping purposes): Title (field_audio_links:title)   
body - Review Body (field_review_body)

For me, my CSV file looked like this - the headers are on the first line, and then bellow that are two sample music reviews.

```
key|artist|title|labelinfo|img|audio|audio-title|body
areview0|A|s/t|"(Die Schachtel) cd 18.98"|https://web.archive.org/web/20160410015723im_/http://aquariusrecords.org/images/aacd.jpg|https://web.archive.org/web/20160410015723/http://aquariusrecords.org/audio/aamemory.m3u;;https://web.archive.org/web/20160410015723/http://aquariusrecords.org/audio/aaremembering.m3u;;https://web.archive.org/web/20160410015723/http://aquariusrecords.org/audio/aasmelltrack.m3u|"""My Memory Is Like A Film. That Is Why"";;""I Am Really Good At Remembering Things, Like The Conversation I Have Written Down In This Book, And What People Are Wearing, And What They Smelled Like, Because My Memory Has"";;""A Smell Track Which Is Like A Soundtrack, And When People Ask Me To Remember Something I Can Simply"""|"Previously the Die Schachtel label has brought us several very cool reissues of some very obscure '70s art/prog/avant music from Italy -- such as Luciano Cilio, Prima Materia and Insiememusicadiversa. Weird and wonderful stuff. Now, they've got a brand new band for us, not a reissue (though it totally seems to fit in with their ""thing""). It's apparently the first in a new series called Zeit.

"
areview1|"A BOLHA"|"Um Passo A Frente"|"(Lion Productions ) cd 14.98"|https://web.archive.org/web/20160410015723im_/http://aquariusrecords.org/images/abopasscd.jpg|https://web.archive.org/web/20160410015723/http://aquariusrecords.org/audio/abolhatempos.m3u;;https://web.archive.org/web/20160410015723/http://aquariusrecords.org/audio/abolhaaespera.m3u;;https://web.archive.org/web/20160410015723/http://aquariusrecords.org/audio/abolhasemnada.m3u|"""Tempos Constantes"";;""A Espera"";;""Sem Nada"""|"Following the Tetragon highlighted here last list, and other recent releases like Sergius Golowin and Guru Guru, what treat does the meritorious and meticulous reissue label Lion Productions have for us THIS week? Another obscure, freaky gem of dusty vintage, yes, but this time not krautrock, instead it's from another wonderful subgenre, that of Latin American psychedelia! This Brazilian band flourished circa 1965-1978, starting off as a Beatlesy dance pop group called The Bubbles, before changing their name to its Portuguese equivalent A Bolha in 1970 and going for a harder, more progressive rock sound, inspired in part by the bands they'd seen on a trip to England, at the Isle of Wight festival. 
```

Double quotes are used to escape inline quotes. The ;; are delimiters for fields that have multiple values. Each of my fields is separated by a | delimiter character.

Then, if you have fields with multiple values, go to the feeds tamper tab:
* add plugin to any multi value fields (audio url and audio title for me). 
* type = explode
* delimiter = ;;

To export the CSV I enable this `myscraper` module and go to `localhost:888/scrape` and this downloads my file. If you have a large file with a long wait time, you may need to change some settings on your server. I use MAMP, so I went to `/Applications/MAMP/Library/support-files/` and made a copy of `my-huge.cnf` named `my.cnf` and set its `max_allowed_packet` to at least 100. I also set a large `max_execution_time`, `upload_max_filesize` and `memory_limit` in php.ini.

Finally, go to `localhost:8888/import` and load the .csv file!


## Known Bugs

This module is (by necessity) very specific to the exact HTML structure of the content on the site that it was built to export. For example most of it was just a series of paragraphs, so for loading content from a site based around `<div>`s the looping would have to be modified.  

## Support and contact details

Please feel free to fork this repository and submit pull requests back. You can also contact me here:

* Email: diane.douglas1@gmail.com
* My Website: [www.MelodicCode.com](http://www.melodiccode.com)

## Technologies Used

* Drupal, PHP.
* Feeds module, Feeds Tamper module and dependencies.
* CSV files.
* External HTML document parsing.

**By Diane Douglas**