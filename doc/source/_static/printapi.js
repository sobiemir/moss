/*
 *  Moss Library >>> http://moss.aculo.pl
 *
 *     /'\_/`\                           
 *    /\      \    ___     ____    ____  
 *    \ \ \__\ \  / __`\  /',__\  /',__\ 
 *     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 *      \ \_\\ \_\ \____/\/\____/\/\____/
 *       \/_/ \/_/\/___/  \/___/  \/___/ 
 *
 *  This file is part of Moss Library.
 *  See LICENSE file for copyright information.
 */

$(function (){
	function getList( selector )
	{
		var list = $(selector);
		var fmt  = [];

		if( !list.length )
			return [];
		
		list.each( function(idx, elem) {
			var el = [];
			var desc = $(elem).children(".descname");
			var href = $(elem).children(".headerlink");

			if( desc.length )
				el.push( desc[0].innerHTML );
			else
				el.push( "undefined" );
			if( href.length )
				el.push( href[0].href );
			else
				el.push( "#undefined" );

			fmt.push( el );
		});

		return fmt;
	}

	var x = [];
	x.push(['type','dl.type > dt']);
	x.push(['func','dl.function > dt']);
	x.push(['var','dl.var > dt']);
	x.push(['macro','dl.macro > dt']);

	x.forEach( function(elem) {
		var list = getList( elem[1] );

		if( !list.length )
			return;

		var apimember = $( "div.api-members div.div-" + elem[0] );
		var apiul     = apimember.children("ul");

		apimember.removeClass( "hidden" );

		list.forEach( function(pos) {
			var a = $('<a href="' + pos[1] + '">');
			a.html( "&raquo; " + pos[0] );
			var li = $('<li class="api-member">').append( a );
			li.append( a );

			apiul.append( li );
		});
	});
});
