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
 *  See LICENSE file for copyright information.
 */

$(function (){
    let list = $('.sphinxsidebar .sphinxsidebarwrapper div ul.this-page-menu');

    // dodaj linki dla aktualnej strony
    $('div.gitlinks').children().each(function(idx, elem) {
        $(elem).children().appendTo( list );
    });
});
