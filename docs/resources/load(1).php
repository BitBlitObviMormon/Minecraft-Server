var mediaWikiLoadStart=(new Date()).getTime(),mwPerformance=(window.performance&&performance.mark)?performance:{mark:function(){}};mwPerformance.mark('mwLoadStart');function isCompatible(str){var ua=str||navigator.userAgent;return!!('querySelector'in document&&'localStorage'in window&&'addEventListener'in window&&!(ua.match(/webOS\/1\.[0-4]/)||ua.match(/PlayStation/i)||ua.match(/SymbianOS|Series60|NetFront|Opera Mini|S40OviBrowser|MeeGo/)||(ua.match(/Glass/)&&ua.match(/Android/))));}(function(){var NORLQ,script;if(!isCompatible()){document.documentElement.className=document.documentElement.className.replace(/(^|\s)client-js(\s|$)/,'$1client-nojs$2');NORLQ=window.NORLQ||[];while(NORLQ.length){NORLQ.shift()();}window.NORLQ={push:function(fn){fn();}};window.RLQ={push:function(){}};return;}function startUp(){mw.config=new mw.Map(true);mw.loader.addSource({"local":"/load.php"});mw.loader.register([["site","1hxdors",[1]],["site.styles","0n9nwt6",[],"site"],["noscript","1l8n5bd",[],"noscript"
],["filepage","1wtyyx6"],["user.groups","1n55y1o",[5]],["user","1fvf5qc",[6],"user"],["user.styles","1drhojy",[],"user"],["user.cssprefs","09p30q0",[],"private"],["user.defaults","0a4hs4k"],["user.options","0j3lz3q",[8],"private"],["user.tokens","0fqh6g4",[],"private"],["mediawiki.language.data","0msaucq",[179]],["mediawiki.skinning.elements","0tw8d4v"],["mediawiki.skinning.content","19e7c9y"],["mediawiki.skinning.interface","1i770c4"],["mediawiki.skinning.content.parsoid","0h1m0t5"],["mediawiki.skinning.content.externallinks","1c1olbx"],["jquery.accessKeyLabel","18atp3x",[27,136]],["jquery.appear","186hlw3"],["jquery.arrowSteps","08qm0ou"],["jquery.async","0s9naiz"],["jquery.autoEllipsis","04lai49",[39]],["jquery.badge","06yzifq",[176]],["jquery.byteLength","1wjrjfq"],["jquery.byteLimit","13cid2h",[23]],["jquery.checkboxShiftClick","066ue0d"],["jquery.chosen","06el049"],["jquery.client","11ic3og"],["jquery.color","0gpblfp",[29]],["jquery.colorUtil","17ppxcp"],["jquery.confirmable",
"0wywjmu",[180]],["jquery.cookie","0fqt22s"],["jquery.expandableField","130j305"],["jquery.farbtastic","13rqkvo",[29]],["jquery.footHovzer","01w0ii4"],["jquery.form","1v3dj5d"],["jquery.fullscreen","077mrhx"],["jquery.getAttrs","0iokof2"],["jquery.hidpi","05hfflr"],["jquery.highlightText","1a1977z",[251,136]],["jquery.hoverIntent","0d0avz3"],["jquery.i18n","142lju6",[178]],["jquery.localize","1lwfccx"],["jquery.makeCollapsible","1ug4gnk"],["jquery.mockjax","0j3suvs"],["jquery.mw-jump","0ueipy2"],["jquery.mwExtension","109lp52"],["jquery.placeholder","0dbsqyh"],["jquery.qunit","1vb46i4"],["jquery.qunit.completenessTest","0oh7cks",[48]],["jquery.spinner","0ygzv1f"],["jquery.jStorage","0agpli9",[94]],["jquery.suggestions","08do68i",[39]],["jquery.tabIndex","0ka4rzn"],["jquery.tablesorter","19cf7wf",[251,136,181]],["jquery.textSelection","1uo92ks",[27]],["jquery.throttle-debounce","0hjxkxk"],["jquery.xmldom","0resztv"],["jquery.tipsy","1cpdv1a"],["jquery.ui.core","1xdivnt",[60],"jquery.ui"
],["jquery.ui.core.styles","1is0ll7",[],"jquery.ui"],["jquery.ui.accordion","1wst7fc",[59,79],"jquery.ui"],["jquery.ui.autocomplete","05i3hqp",[68],"jquery.ui"],["jquery.ui.button","0lei5bs",[59,79],"jquery.ui"],["jquery.ui.datepicker","0rjcuvj",[59],"jquery.ui"],["jquery.ui.dialog","0ptfubc",[63,66,70,72],"jquery.ui"],["jquery.ui.draggable","121zuhq",[59,69],"jquery.ui"],["jquery.ui.droppable","1hq319n",[66],"jquery.ui"],["jquery.ui.menu","0n4m6mm",[59,70,79],"jquery.ui"],["jquery.ui.mouse","1s786ym",[79],"jquery.ui"],["jquery.ui.position","0kz9qsn",[],"jquery.ui"],["jquery.ui.progressbar","0klbopk",[59,79],"jquery.ui"],["jquery.ui.resizable","1uy0di9",[59,69],"jquery.ui"],["jquery.ui.selectable","128lnbi",[59,69],"jquery.ui"],["jquery.ui.slider","11rxnua",[59,69],"jquery.ui"],["jquery.ui.sortable","16oay7i",[59,69],"jquery.ui"],["jquery.ui.spinner","13wefiu",[63],"jquery.ui"],["jquery.ui.tabs","0vhrqud",[59,79],"jquery.ui"],["jquery.ui.tooltip","0osx7et",[59,70,79],"jquery.ui"],[
"jquery.ui.widget","0da76xx",[],"jquery.ui"],["jquery.effects.core","0h4en75",[],"jquery.ui"],["jquery.effects.blind","0kts15s",[80],"jquery.ui"],["jquery.effects.bounce","04p4qgk",[80],"jquery.ui"],["jquery.effects.clip","010desl",[80],"jquery.ui"],["jquery.effects.drop","1mbdm0p",[80],"jquery.ui"],["jquery.effects.explode","0ca0r3u",[80],"jquery.ui"],["jquery.effects.fade","0g5tx9s",[80],"jquery.ui"],["jquery.effects.fold","0pgc9v8",[80],"jquery.ui"],["jquery.effects.highlight","0jox706",[80],"jquery.ui"],["jquery.effects.pulsate","0klxkie",[80],"jquery.ui"],["jquery.effects.scale","0qd5z8y",[80],"jquery.ui"],["jquery.effects.shake","0hv30e9",[80],"jquery.ui"],["jquery.effects.slide","16ty03y",[80],"jquery.ui"],["jquery.effects.transfer","1uv8y58",[80],"jquery.ui"],["json","121h06d",[],null,null,"return!!(window.JSON\u0026\u0026JSON.stringify\u0026\u0026JSON.parse);"],["moment","1io8w9j",[176]],["mediawiki.apihelp","09o71ar"],["mediawiki.template","0ga2c2p"],[
"mediawiki.template.mustache","0hvm1xz",[97]],["mediawiki.template.regexp","18qgw7b",[97]],["mediawiki.apipretty","1k75b3z"],["mediawiki.api","1iklkpc",[153,10]],["mediawiki.api.category","14btjpc",[141,101]],["mediawiki.api.edit","0nz1ll6",[141,101]],["mediawiki.api.login","0zs0tj8",[101]],["mediawiki.api.options","0swbcw3",[101]],["mediawiki.api.parse","1f0hamp",[101]],["mediawiki.api.upload","1kpir5h",[251,94,103]],["mediawiki.api.user","0nrty82",[101]],["mediawiki.api.watch","1964fkn",[101]],["mediawiki.api.messages","1ymrn9e",[101]],["mediawiki.api.rollback","071glsk",[101]],["mediawiki.content.json","05m97st"],["mediawiki.confirmCloseWindow","186gzvx"],["mediawiki.debug","07jpddw",[34]],["mediawiki.diff.styles","0bzyha4"],["mediawiki.feedback","0fyrcdp",[141,130,260]],["mediawiki.feedlink","1v0b34u"],["mediawiki.filewarning","11cn84h",[256]],["mediawiki.ForeignApi","10y6k6i",[120]],["mediawiki.ForeignApi.core","1ndhvxd",[101,252]],["mediawiki.helplink","0ytkue8"],[
"mediawiki.hidpi","0wlluls",[38],null,null,"return'srcset'in new Image();"],["mediawiki.hlist","0posn83"],["mediawiki.htmlform","1tlmuwl",[24,136]],["mediawiki.htmlform.ooui","13d162o",[256]],["mediawiki.htmlform.styles","0pm8n7w"],["mediawiki.htmlform.ooui.styles","1iy5idf"],["mediawiki.icon","0y57uae"],["mediawiki.inspect","1tdenel",[23,94,136]],["mediawiki.messagePoster","0q39qzi",[119]],["mediawiki.messagePoster.wikitext","1wuob2c",[103,130]],["mediawiki.notification","0exss5e",[189]],["mediawiki.notify","1g76zc8"],["mediawiki.notification.convertmessagebox","1hocx7p",[132]],["mediawiki.notification.convertmessagebox.styles","0f3qjly"],["mediawiki.RegExp","18s5l66"],["mediawiki.pager.tablePager","0a46w46"],["mediawiki.searchSuggest","0am0kqg",[37,47,52,101]],["mediawiki.sectionAnchor","12kyfcx"],["mediawiki.storage","0oxgzh6"],["mediawiki.Title","065drjx",[23,153]],["mediawiki.Upload","01283yj",[107]],["mediawiki.ForeignUpload","0wnbkfc",[119,142]],[
"mediawiki.ForeignStructuredUpload.config","1fvsh13"],["mediawiki.ForeignStructuredUpload","1y3z92b",[144,143]],["mediawiki.Upload.Dialog","1b3tdyc",[147]],["mediawiki.Upload.BookletLayout","00t17fo",[142,180,151,249,95,258,260,266,267]],["mediawiki.ForeignStructuredUpload.BookletLayout","05yl8ty",[145,147,110,184,245,243]],["mediawiki.toc","1keop92",[157]],["mediawiki.Uri","1pt8vtr",[153,99]],["mediawiki.user","1ncg4tw",[108,157,9]],["mediawiki.userSuggest","0p966cn",[52,101]],["mediawiki.util","10yadzg",[17,133]],["mediawiki.viewport","1lzmtdo"],["mediawiki.checkboxtoggle","0m8bd3l"],["mediawiki.checkboxtoggle.styles","1po4uhx"],["mediawiki.cookie","0lwmuda",[31]],["mediawiki.toolbar","1w8pa4z",[55]],["mediawiki.experiments","0z5mebk"],["mediawiki.action.edit","12dhp3k",[24,55,161,101]],["mediawiki.action.edit.styles","027rfq7"],["mediawiki.action.edit.collapsibleFooter","10au7lp",[43,157,128]],["mediawiki.action.edit.preview","0amqbpf",[35,50,55,101,115,180]],[
"mediawiki.action.history","12lt0qx"],["mediawiki.action.history.styles","1qreetx"],["mediawiki.action.history.diff","0bzyha4"],["mediawiki.action.view.dblClickEdit","0e8rjru",[189,9]],["mediawiki.action.view.metadata","1t9ngj2"],["mediawiki.action.view.categoryPage.styles","1wpp94l"],["mediawiki.action.view.postEdit","0dhgrqe",[157,180,97]],["mediawiki.action.view.redirect","02otdhj",[27]],["mediawiki.action.view.redirectPage","1qg8y2n"],["mediawiki.action.view.rightClickEdit","0njlxdi"],["mediawiki.action.edit.editWarning","0brd56u",[55,113,180]],["mediawiki.action.view.filepage","1dzy2h6"],["mediawiki.language","0y40evi",[177,11]],["mediawiki.cldr","0nbnnt5",[178]],["mediawiki.libs.pluralruleparser","1wnz6rj"],["mediawiki.language.init","0sar8w2"],["mediawiki.jqueryMsg","0jm4mo7",[251,176,153,9]],["mediawiki.language.months","0qyautx",[176]],["mediawiki.language.names","16yzhn6",[179]],["mediawiki.language.specialCharacters","06es0c8",[176]],["mediawiki.libs.jpegmeta","08lxn56"],[
"mediawiki.page.gallery","1bs4lqt",[56,186]],["mediawiki.page.gallery.styles","1qy60qv"],["mediawiki.page.gallery.slideshow","0yc7zup",[141,101,258,274]],["mediawiki.page.ready","1c6onpd",[17,25,43,45,47]],["mediawiki.page.startup","1c1c0i4",[153]],["mediawiki.page.patrol.ajax","09njj50",[50,141,101,189]],["mediawiki.page.watch.ajax","1smhcn0",[109,189]],["mediawiki.page.rollback","1ofq2n6",[50,111]],["mediawiki.page.image.pagination","17onsmb",[50,153]],["mediawiki.special","0ew6x4z"],["mediawiki.special.apisandbox.styles","0i5ygz3"],["mediawiki.special.apisandbox","1e2l83s",[101,180,244,255]],["mediawiki.special.block","0baf08e",[153]],["mediawiki.special.changeslist","0bickjq"],["mediawiki.special.changeslist.legend","0nwkui7"],["mediawiki.special.changeslist.legend.js","04jch2h",[43,157]],["mediawiki.special.changeslist.enhanced","0j5emj6"],["mediawiki.special.changeslist.visitedstatus","123wm2v"],["mediawiki.special.comparepages.styles","0efwm7t"],["mediawiki.special.edittags",
"1xitvum",[26]],["mediawiki.special.edittags.styles","1ucw585"],["mediawiki.special.import","1tomvxk"],["mediawiki.special.movePage","05qs14h",[241]],["mediawiki.special.movePage.styles","07vxfdl"],["mediawiki.special.pageLanguage","0s9bgut",[256]],["mediawiki.special.pagesWithProp","0492mna"],["mediawiki.special.preferences","1ymycoe",[113,176,134]],["mediawiki.special.userrights","06aqjy1",[134]],["mediawiki.special.preferences.styles","0yblgqd"],["mediawiki.special.recentchanges","01scvd1"],["mediawiki.special.search","0dng0ji",[247]],["mediawiki.special.search.styles","1yu6ost"],["mediawiki.special.undelete","08cg6iw"],["mediawiki.special.upload","1gqezlw",[50,141,101,113,180,184,219,97]],["mediawiki.special.upload.styles","18yeg94"],["mediawiki.special.userlogin.common.styles","0ccw60z"],["mediawiki.special.userlogin.signup.styles","1019ki6"],["mediawiki.special.userlogin.login.styles","121qje7"],["mediawiki.special.userlogin.signup.js","0glg0kt",[56,101,180]],[
"mediawiki.special.unwatchedPages","0p3po2m",[141,109]],["mediawiki.special.watchlist","12yhh6s"],["mediawiki.special.version","1og3d9u"],["mediawiki.legacy.config","12h9il7"],["mediawiki.legacy.commonPrint","1kr9ndv"],["mediawiki.legacy.protect","1oimdif",[24]],["mediawiki.legacy.shared","0wermoc"],["mediawiki.legacy.oldshared","0znx0x0"],["mediawiki.legacy.wikibits","0323xwk",[153]],["mediawiki.ui","1gr5isr"],["mediawiki.ui.checkbox","02jd2hs"],["mediawiki.ui.radio","0hfmto1"],["mediawiki.ui.anchor","0czrvg6"],["mediawiki.ui.button","0nmgbp0"],["mediawiki.ui.input","1yzkcs1"],["mediawiki.ui.icon","1lotuq6"],["mediawiki.ui.text","10vk97p"],["mediawiki.widgets","1r0v5fw",[21,24,141,101,242,258]],["mediawiki.widgets.styles","033re9b"],["mediawiki.widgets.DateInputWidget","0p4akj9",[95,258]],["mediawiki.widgets.datetime","1b2uxsi",[256]],["mediawiki.widgets.CategorySelector","15dqx1j",[119,141,258]],["mediawiki.widgets.UserInputWidget","1iukry1",[258]],[
"mediawiki.widgets.SearchInputWidget","0pc06x3",[138,241]],["mediawiki.widgets.SearchInputWidget.styles","12u3685"],["mediawiki.widgets.StashedFileWidget","008ofmz",[256]],["es5-shim","04yi9an",[],null,null,"return(function(){'use strict';return!this\u0026\u0026!!Function.prototype.bind;}());"],["dom-level2-shim","12wdk8x",[],null,null,"return!!window.Node;"],["oojs","151cwjs",[250,94]],["mediawiki.router","0n8p84v",[254]],["oojs-router","1gl9m51",[252]],["oojs-ui","1n55y1o",[259,258,260]],["oojs-ui-core","015a03g",[176,252,257,261,262,263]],["oojs-ui-core.styles","04yyl3z"],["oojs-ui-widgets","150b7hs",[256]],["oojs-ui-toolbars","1vaeglw",[256]],["oojs-ui-windows","0r7axp9",[256]],["oojs-ui.styles.icons","16mn0le"],["oojs-ui.styles.indicators","0wzjur2"],["oojs-ui.styles.textures","0ldimde"],["oojs-ui.styles.icons-accessibility","19ix1t8"],["oojs-ui.styles.icons-alerts","1n5e4m5"],["oojs-ui.styles.icons-content","1nneuk5"],["oojs-ui.styles.icons-editing-advanced","1tc1b9q"],[
"oojs-ui.styles.icons-editing-core","024arfm"],["oojs-ui.styles.icons-editing-list","0dxuu4z"],["oojs-ui.styles.icons-editing-styling","11eie0e"],["oojs-ui.styles.icons-interactions","02c9qmq"],["oojs-ui.styles.icons-layout","04fd1hy"],["oojs-ui.styles.icons-location","1rzzo6y"],["oojs-ui.styles.icons-media","1prykgp"],["oojs-ui.styles.icons-moderation","0gaox9z"],["oojs-ui.styles.icons-movement","1kg2ply"],["oojs-ui.styles.icons-user","1t4qr7g"],["oojs-ui.styles.icons-wikimedia","1mvt9ha"],["skins.cologneblue","0r6lcgv"],["skins.modern","0k0fvrg"],["skins.monobook.styles","1y51e25"],["skins.vector.styles","05i4tfy"],["skins.vector.styles.responsive","11potvn"],["skins.vector.js","11bf37y",[53,56]],["ext.pygments","0c7asck"],["ext.nuke","0hi665w"],["ext.checkUser","0ln6dkj",[153]],["ext.checkUser.caMultiLock","122ogn9",[153]],["ext.confirmEdit.editPreview.ipwhitelist.styles","0fg2foy"],["ext.cite.styles","01j9hk3"],["ext.cite.a11y","08nj3qk"],["ext.cite.style","19t2lim"],[
"mediawiki.api.titleblacklist","0xhsjt6",[101]],["ext.SmiteSpam.retriever","1q5tvoz",[50,180]]]);;mw.config.set({"wgLoadScript":"/load.php","debug":!1,"skin":"vector","stylepath":"/skins","wgUrlProtocols":"bitcoin\\:|ftp\\:\\/\\/|ftps\\:\\/\\/|geo\\:|git\\:\\/\\/|gopher\\:\\/\\/|http\\:\\/\\/|https\\:\\/\\/|irc\\:\\/\\/|ircs\\:\\/\\/|magnet\\:|mailto\\:|mms\\:\\/\\/|news\\:|nntp\\:\\/\\/|redis\\:\\/\\/|sftp\\:\\/\\/|sip\\:|sips\\:|sms\\:|ssh\\:\\/\\/|svn\\:\\/\\/|tel\\:|telnet\\:\\/\\/|urn\\:|worldwind\\:\\/\\/|xmpp\\:|\\/\\/","wgArticlePath":"/$1","wgScriptPath":"","wgScriptExtension":".php","wgScript":"/index.php","wgSearchType":null,"wgVariantArticlePath":!1,"wgActionPaths":{},"wgServer":"http://wiki.vg","wgServerName":"wiki.vg","wgUserLanguage":"en","wgContentLanguage":"en","wgTranslateNumerals":!0,"wgVersion":"1.28.2","wgEnableAPI":!0,"wgEnableWriteAPI":!0,"wgMainPageTitle":"Main Page","wgFormattedNamespaces":{"-2":"Media","-1":"Special","0":"","1":"Talk","2":"User",
"3":"User talk","4":"wikivg","5":"wikivg talk","6":"File","7":"File talk","8":"MediaWiki","9":"MediaWiki talk","10":"Template","11":"Template talk","12":"Help","13":"Help talk","14":"Category","15":"Category talk"},"wgNamespaceIds":{"media":-2,"special":-1,"":0,"talk":1,"user":2,"user_talk":3,"wikivg":4,"wikivg_talk":5,"file":6,"file_talk":7,"mediawiki":8,"mediawiki_talk":9,"template":10,"template_talk":11,"help":12,"help_talk":13,"category":14,"category_talk":15,"image":6,"image_talk":7,"project":4,"project_talk":5},"wgContentNamespaces":[0],"wgSiteName":"wiki.vg","wgDBname":"wiki","wgExtraSignatureNamespaces":[],"wgAvailableSkins":{"cologneblue":"CologneBlue","modern":"Modern","monobook":"MonoBook","vector":"Vector","fallback":"Fallback","apioutput":"ApiOutput"},"wgExtensionAssetsPath":"/extensions","wgCookiePrefix":"wiki_wiki_","wgCookieDomain":"","wgCookiePath":"/","wgCookieExpiration":15552000,"wgResourceLoaderMaxQueryLength":-1,"wgCaseSensitiveNamespaces":[],"wgLegalTitleChars":
" %!\"$&'()*,\\-./0-9:;=?@A-Z\\\\\\^_`a-z~+\\u0080-\\uFFFF","wgIllegalFileChars":":/\\\\","wgResourceLoaderStorageVersion":1,"wgResourceLoaderStorageEnabled":!0,"wgResourceLoaderLegacyModules":[],"wgForeignUploadTargets":["local"],"wgEnableUploads":!0,"wgCiteVisualEditorOtherGroup":!1});var RLQ=window.RLQ||[];while(RLQ.length){RLQ.shift()();}window.RLQ={push:function(fn){fn();}};window.NORLQ={push:function(){}};}script=document.createElement('script');script.src="/load.php?debug=false&lang=en&modules=jquery%2Cmediawiki&only=scripts&skin=vector&version=0xdj67j";script.onload=script.onreadystatechange=function(){if(!script.readyState||/loaded|complete/.test(script.readyState)){script.onload=script.onreadystatechange=null;script=null;startUp();}};document.getElementsByTagName('head')[0].appendChild(script);}());