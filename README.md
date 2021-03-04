@font-your-face
===============

@font-your-face provides an administrative interface for browsing and applying web fonts (using CSS @font-face, supported in all popular browsers) from a variety of sources.

Features
--------

* A font browsing interface allows selection of fonts from from several providers:
  * Typekit.com
  * Google Fonts
  * Adobe Edge Web Fonts
  * Font Squirrel
  * Fonts.com
* Fonts are automatically loaded on site simply by clicking "Enable"; no need to create font files, write CSS, add JS, nor figure out different methods for every font provider.
* Clear indication of license restrictions on provided fonts.
* Fonts can be applied to specific text on the site either by using the provided font-family identifier in theme CSS, or by typing a CSS selector directly in the @font-your-face interface.
* The @font-your-face code is designed for re-use, so other modules can both supply and read active and available font information.
* You can also import your own local fonts in WOFF format.)

Installing
----------

* Install this module using the official Backdrop CMS instructions at <https://backdropcms.org/guide/modules>.
* Place the extracted module in sites/all/modules/fontyourface
* Go to Administration » Modules and enable @font-your-face and one or more of the submodules.
* Go to Administration » Configuration » User interface » @font-your-face settings and import the fonts.

Use @font-your-face
-------------------

* Go to Administration » Appearance » @font-your-face (admin/appearance/fontyourface/browse) to enable some fonts.
* Click the 'enable font' for each font you want to use.
* You can add CSS selectors for each enabled font.

Using @font-your-face via your theme .info file
-----------------------------------------------

* Open the .info file of your theme (eg basis.info if your theme is Basis).
* Add fonts like this:

```
fonts[google_fonts_api][] = "Contrail One&subset=latin#regular"
fonts[fontdeck][] = "Tanger+Serif+Medium+Ultra+Light"
```

* The use your stylesheet to enable fonts. Example:

  `h1#site-title { font-family: "1942 Report", serif; }`

Known issues
------------

Note that Internet Explorer has a limit of 32 CSS files, so using @font-your-face on CSS-heavy sites may require turning on CSS aggregation under Administer » Configuration » Development » Performance (admin/config/development/performance).

KERNEST servers are sometimes unreliable, so you may want to download KERNEST fonts and use the Local Fonts module to load them from your server instead. KERNEST provides paid fonts that are not available for use in the API, so those can only be used with the Local Fonts module.

Issues
------

To submit bug reports and feature suggestions, or to track changes:
  <https://github.com/backdrop-contrib/fontyourface/issues>

Current Maintainers
-------------------

* Herb v/d Dool <https://github.com/herbdool/>
* Seeking additional maintainers.

Credits
-------

* Ported to Backdrop by Herb v/d Dool <https://github.com/herbdool/>
* Originally developed for Drupal by [sreynen](https://www.drupal.org/u/sreynen), [Neslee Canil Pinto](https://www.drupal.org/u/neslee-canil-pinto), [BTMash](https://www.drupal.org/u/btmash), [Drave Robber](https://www.drupal.org/u/drave-robber), [BarisW](https://www.drupal.org/u/barisw), .

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
