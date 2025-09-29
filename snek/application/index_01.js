var enableADS = false;
dataLayer = [];
var productKey = "";

dataLayer.push({
  'productKey': '' + productKey
});

function gtm()
{
	(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
	new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
	j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
	'//www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
	})(window,document,'script','dataLayer','GTM-W4F73N');
}
gtm();

function gtm_noads()
{
	(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
	new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
	j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
	'//www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
	})(window,document,'script','dataLayer','GTM-K74X66');
}
gtm_noads();

var fbAppID = "871310506370221";

onload = function () {
	var e = document.getElementById("page_is_dirty");
	if (e.value == "no") e.value = "yes";
	else { e.value = "no"; location.reload(); }
}

if(typeof(canDetectPack) != "undefined")
{
	var detectLink = getURL();
	var currLink = window.location.href;
	currLink = currLink.replace("index.html","");
	if(detectLink.toLowerCase() != currLink.toLowerCase())
		window.location.href = detectLink;
}
else if(false/*remove by Construct2 Supporter Step 1*/)
{
	var isUCBrowser = /UCBrowser/i.test(navigator.userAgent);
	if(isUCBrowser)
	{
	   if(window.location.href.indexOf("index_UC") < 0)
			window.location.href = "index_UC.html";
	}
	else
	{
		if(window.location.href.indexOf("index_UC") > 0)
			window.location.href = "index.html";
	}
	var isIE = /msie/i.test(navigator.userAgent) || /trident/i.test(navigator.userAgent) || /iemobile/i.test(navigator.userAgent);
	var isiPhone = (/iphone/i.test(navigator.userAgent) || /ipod/i.test(navigator.userAgent)) && !isIE;	// treat ipod as an iphone; IE mobile masquerades as iPhone
	var isiPad = /ipad/i.test(navigator.userAgent);
	var isiOS = isiPhone || isiPad;
	var IsAsusHuawei_AndroidBrowser = (/Huawei/i.test(navigator.userAgent)
										&& /Version\//i.test(navigator.userAgent)
										)
	if(isiOS || IsAsusHuawei_AndroidBrowser)
	{
	   if(window.location.href.indexOf("index_iPhone") < 0)
	   {
			var sp = window.location.href.split("?");
			if(sp.length > 1)
				window.location.href = "index_iPhone.html" + "?" + sp[1];
			else
				window.location.href = "index_iPhone.html";
		}
	}
	else
	{
		if(window.location.href.indexOf("index_iPhone") > 0)
		{
			var sp = window.location.href.split("?");
			if(sp.length > 1)
				window.location.href = "index.html" + "?" + sp[1];
			else
				window.location.href = "index.html";
		}
	}
}
function loadingError()
{
	// document.getElementById("loading_gif").src = "http://play.ludigames.com/js/games_lib/freeFiles/loading.gif";
}

if(!window.enableADS) {
	var loading_image = document.getElementById("loading_gif");
	if(loading_image != null) {
		loading_image.style.display = "inline";
	}
}
		

// Size the canvas to fill the browser viewport.
function createC2canvas(){
	// var goToADS = true;
	// if(typeof(window.GameStarted) == "undefined")
		// goToADS = false;
	// else if(!window.enableADS)
		// goToADS = false;
	// if(!goToADS)
	{
		// document.getElementById("loading_gif").style.display = "inline";
		var loading_image = document.getElementById("loading_gif");
		if(loading_image != null) {
			loading_image.style.display = "inline";
		}
		jQuery(window).resize(function() {
			cr_sizeCanvas(jQuery(window).width(), jQuery(window).height());
		});
		
		// Start the Construct 2 project running on window load.
		jQuery(document).ready(function ()
		{			
			// Create new runtime using the c2canvas
			cr_createRuntime("c2canvas");
		});
		
		// Pause and resume on page becoming visible/invisible
		function onVisibilityChanged() {
			if (document.hidden || document.mozHidden || document.webkitHidden || document.msHidden)
				cr_setSuspended(true);
			else
				cr_setSuspended(false);
		};
		
		document.addEventListener("visibilitychange", onVisibilityChanged, false);
		document.addEventListener("mozvisibilitychange", onVisibilityChanged, false);
		document.addEventListener("webkitvisibilitychange", onVisibilityChanged, false);
		document.addEventListener("msvisibilitychange", onVisibilityChanged, false);
	}
}
createC2canvas();

function OnRegisterSWError(e)
{
	console.warn("Failed to register service worker: ", e);
};

// Runtime calls this global method when ready to start caching (i.e. after startup).
// This registers the service worker which caches resources for offline support.
window.C2_RegisterSW = function C2_RegisterSW()
{
	if (!navigator.serviceWorker)
		return;		// no SW support, ignore call
	
	try {
		navigator.serviceWorker.register("sw.js", { scope: "./" })
		.then(function (reg)
		{
			console.log("Registered service worker on " + reg.scope);
		})
		.catch(OnRegisterSWError);
	}
	catch (e)
	{
		OnRegisterSWError(e);
	}
};

function getPixelsPerCm()
{
	/*var o = document.getElementById("device_physic_pixels_per_cm");
	alert(o.offsetWidth)
	return o.offsetWidth
	*/
	return 38;
};
    