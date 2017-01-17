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