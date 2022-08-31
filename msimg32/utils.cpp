#include "pch.h"
#include "utils.h"

string read_to_string(const string& path) {
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return string(buffer.str());
}

string ws2s(const wstring& str) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(str);
}

std::wstring s2ws(const std::string& s, bool isUtf8)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(isUtf8 ? CP_UTF8 : CP_ACP, 0, s.c_str(), slength, 0, 0);
	std::wstring buf;
	buf.resize(len);
	MultiByteToWideChar(isUtf8 ? CP_UTF8 : CP_ACP, 0, s.c_str(), slength,
		const_cast<wchar_t*>(buf.c_str()), len);
	return buf;
}


bool check_legal_file_path(const string& path) {
	return pystring::find(path, "..") == -1;
}

void write_file_text(const string& path, const string& text) {
	ofstream file;
	file.open(path);
	file << text;
	file.close();
}

string switchInput = R"(
;(()=>{var e={778:(e,t,r)=>{"use strict";r.d(t,{Z:()=>s});var n=r(81),o=r.n(n),i=r(645),a=r.n(i)()(o());a.push([e.id,'.switch{--line: #c67ed0;--dot: #9ff7c1;--circle: rgb(224, 150, 170);--duration: .3s;--text: #9EA0BE;cursor:pointer}.switch input{display:none}.switch input+div{position:relative}.switch input+div:before,.switch input+div:after{--s: 1;content:"";position:absolute;height:4px;top:10px;width:24px;background:var(--line);transform:scaleX(var(--s));transition:transform var(--duration) ease}.switch input+div:before{--s: 0;left:0;transform-origin:0 50%;border-radius:2px 0 0 2px}.switch input+div:after{left:28px;transform-origin:100% 50%;border-radius:0 2px 2px 0}.switch input+div span{padding-left:56px;line-height:24px;color:var(--text)}.switch input+div span:before{--x: 0;--b: var(--circle);--s: 4px;content:"";position:absolute;left:0;top:0;width:24px;height:24px;border-radius:50%;box-shadow:inset 0 0 0 var(--s) var(--b);transform:translateX(var(--x));transition:box-shadow var(--duration) ease,transform var(--duration) ease}.switch input+div span:not(:empty){padding-left:64px}.switch input:checked+div:before{--s: 1}.switch input:checked+div:after{--s: 0}.switch input:checked+div span:before{--x: 28px;--s: 12px;--b: var(--dot)}',""]);const s=a},645:e=>{"use strict";e.exports=function(e){var t=[];return t.toString=function(){return this.map((function(t){var r="",n=void 0!==t[5];return t[4]&&(r+="@supports (".concat(t[4],") {")),t[2]&&(r+="@media ".concat(t[2]," {")),n&&(r+="@layer".concat(t[5].length>0?" ".concat(t[5]):""," {")),r+=e(t),n&&(r+="}"),t[2]&&(r+="}"),t[4]&&(r+="}"),r})).join("")},t.i=function(e,r,n,o,i){"string"==typeof e&&(e=[[null,e,void 0]]);var a={};if(n)for(var s=0;s<this.length;s++){var c=this[s][0];null!=c&&(a[c]=!0)}for(var u=0;u<e.length;u++){var p=[].concat(e[u]);n&&a[p[0]]||(void 0!==i&&(void 0===p[5]||(p[1]="@layer".concat(p[5].length>0?" ".concat(p[5]):""," {").concat(p[1],"}")),p[5]=i),r&&(p[2]?(p[1]="@media ".concat(p[2]," {").concat(p[1],"}"),p[2]=r):p[2]=r),o&&(p[4]?(p[1]="@supports (".concat(p[4],") {").concat(p[1],"}"),p[4]=o):p[4]="".concat(o)),t.push(p))}},t}},81:e=>{"use strict";e.exports=function(e){return e[1]}},277:(e,t,r)=>{"use strict";r.r(t),r.d(t,{default:()=>b});var n=r(379),o=r.n(n),i=r(795),a=r.n(i),s=r(569),c=r.n(s),u=r(565),p=r.n(u),d=r(216),l=r.n(d),f=r(589),v=r.n(f),h=r(778),m={};m.styleTagTransform=v(),m.setAttributes=p(),m.insert=c().bind(null,"head"),m.domAPI=a(),m.insertStyleElement=l(),o()(h.Z,m);const b=h.Z&&h.Z.locals?h.Z.locals:void 0},379:e=>{"use strict";var t=[];function r(e){for(var r=-1,n=0;n<t.length;n++)if(t[n].identifier===e){r=n;break}return r}function n(e,n){for(var i={},a=[],s=0;s<e.length;s++){var c=e[s],u=n.base?c[0]+n.base:c[0],p=i[u]||0,d="".concat(u," ").concat(p);i[u]=p+1;var l=r(d),f={css:c[1],media:c[2],sourceMap:c[3],supports:c[4],layer:c[5]};if(-1!==l)t[l].references++,t[l].updater(f);else{var v=o(f,n);n.byIndex=s,t.splice(s,0,{identifier:d,updater:v,references:1})}a.push(d)}return a}function o(e,t){var r=t.domAPI(t);return r.update(e),function(t){if(t){if(t.css===e.css&&t.media===e.media&&t.sourceMap===e.sourceMap&&t.supports===e.supports&&t.layer===e.layer)return;r.update(e=t)}else r.remove()}}e.exports=function(e,o){var i=n(e=e||[],o=o||{});return function(e){e=e||[];for(var a=0;a<i.length;a++){var s=r(i[a]);t[s].references--}for(var c=n(e,o),u=0;u<i.length;u++){var p=r(i[u]);0===t[p].references&&(t[p].updater(),t.splice(p,1))}i=c}}},569:e=>{"use strict";var t={};e.exports=function(e,r){var n=function(e){if(void 0===t[e]){var r=document.querySelector(e);if(window.HTMLIFrameElement&&r instanceof window.HTMLIFrameElement)try{r=r.contentDocument.head}catch(e){r=null}t[e]=r}return t[e]}(e);if(!n)throw new Error("Couldn't find a style target. This probably means that the value for the 'insert' parameter is invalid.");n.appendChild(r)}},216:e=>{"use strict";e.exports=function(e){var t=document.createElement("style");return e.setAttributes(t,e.attributes),e.insert(t,e.options),t}},565:(e,t,r)=>{"use strict";e.exports=function(e){var t=r.nc;t&&e.setAttribute("nonce",t)}},795:e=>{"use strict";e.exports=function(e){var t=e.insertStyleElement(e);return{update:function(r){!function(e,t,r){var n="";r.supports&&(n+="@supports (".concat(r.supports,") {")),r.media&&(n+="@media ".concat(r.media," {"));var o=void 0!==r.layer;o&&(n+="@layer".concat(r.layer.length>0?" ".concat(r.layer):""," {")),n+=r.css,o&&(n+="}"),r.media&&(n+="}"),r.supports&&(n+="}");var i=r.sourceMap;i&&"undefined"!=typeof btoa&&(n+="\n/*# sourceMappingURL=data:application/json;base64,".concat(btoa(unescape(encodeURIComponent(JSON.stringify(i))))," */")),t.styleTagTransform(n,e,t.options)}(t,e,r)},remove:function(){!function(e){if(null===e.parentNode)return!1;e.parentNode.removeChild(e)}(t)}}}},589:e=>{"use strict";e.exports=function(e,t){if(t.styleSheet)t.styleSheet.cssText=e;else{for(;t.firstChild;)t.removeChild(t.firstChild);t.appendChild(document.createTextNode(e))}}}},t={};function r(n){var o=t[n];if(void 0!==o)return o.exports;var i=t[n]={id:n,exports:{}};return e[n](i,i.exports,r),i.exports}r.n=e=>{var t=e&&e.__esModule?()=>e.default:()=>e;return r.d(t,{a:t}),t},r.d=(e,t)=>{for(var n in t)r.o(t,n)&&!r.o(e,n)&&Object.defineProperty(e,n,{enumerable:!0,get:t[n]})},r.o=(e,t)=>Object.prototype.hasOwnProperty.call(e,t),r.r=e=>{"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(e,"__esModule",{value:!0})},r.nc=void 0,r(277),window.createSwitch=function(){const e=document.createElement("label");e.className="switch";const t=document.createElement("input");t.type="checkbox";const r=document.createElement("div");return r.appendChild(document.createElement("span")),e.appendChild(t),e.appendChild(r),e}})();
)";

string customToast = R"(
;(()=>{var t={235:(t,o,n)=>{"use strict";n.d(o,{Z:()=>r});var e=n(81),i=n.n(e),s=n(645),a=n.n(s)()(i());a.push([t.id,"/*!\n * Toastify js 1.12.0\n * https://github.com/apvarun/toastify-js\n * @license MIT licensed\n *\n * Copyright (C) 2018 Varun A P\n */\n\n.toastify {\n    padding: 12px 20px;\n    color: #ffffff;\n    display: inline-block;\n    box-shadow: 0 3px 6px -1px rgba(0, 0, 0, 0.12), 0 10px 36px -4px rgba(77, 96, 232, 0.3);\n    background: -webkit-linear-gradient(315deg, #73a5ff, #5477f5);\n    background: linear-gradient(135deg, #73a5ff, #5477f5);\n    position: fixed;\n    opacity: 0;\n    transition: all 0.4s cubic-bezier(0.215, 0.61, 0.355, 1);\n    border-radius: 2px;\n    cursor: pointer;\n    text-decoration: none;\n    max-width: calc(50% - 20px);\n    z-index: 2147483647;\n}\n\n.toastify.on {\n    opacity: 1;\n}\n\n.toast-close {\n    background: transparent;\n    border: 0;\n    color: white;\n    cursor: pointer;\n    font-family: inherit;\n    font-size: 1em;\n    opacity: 0.4;\n    padding: 0 5px;\n}\n\n.toastify-right {\n    right: 15px;\n}\n\n.toastify-left {\n    left: 15px;\n}\n\n.toastify-top {\n    top: -150px;\n}\n\n.toastify-bottom {\n    bottom: -150px;\n}\n\n.toastify-rounded {\n    border-radius: 25px;\n}\n\n.toastify-avatar {\n    width: 1.5em;\n    height: 1.5em;\n    margin: -7px 5px;\n    border-radius: 2px;\n}\n\n.toastify-center {\n    margin-left: auto;\n    margin-right: auto;\n    left: 0;\n    right: 0;\n    max-width: fit-content;\n    max-width: -moz-fit-content;\n}\n\n@media only screen and (max-width: 360px) {\n    .toastify-right, .toastify-left {\n        margin-left: auto;\n        margin-right: auto;\n        left: 0;\n        right: 0;\n        max-width: fit-content;\n    }\n}\n",""]);const r=a},645:t=>{"use strict";t.exports=function(t){var o=[];return o.toString=function(){return this.map((function(o){var n="",e=void 0!==o[5];return o[4]&&(n+="@supports (".concat(o[4],") {")),o[2]&&(n+="@media ".concat(o[2]," {")),e&&(n+="@layer".concat(o[5].length>0?" ".concat(o[5]):""," {")),n+=t(o),e&&(n+="}"),o[2]&&(n+="}"),o[4]&&(n+="}"),n})).join("")},o.i=function(t,n,e,i,s){"string"==typeof t&&(t=[[null,t,void 0]]);var a={};if(e)for(var r=0;r<this.length;r++){var c=this[r][0];null!=c&&(a[c]=!0)}for(var p=0;p<t.length;p++){var l=[].concat(t[p]);e&&a[l[0]]||(void 0!==s&&(void 0===l[5]||(l[1]="@layer".concat(l[5].length>0?" ".concat(l[5]):""," {").concat(l[1],"}")),l[5]=s),n&&(l[2]?(l[1]="@media ".concat(l[2]," {").concat(l[1],"}"),l[2]=n):l[2]=n),i&&(l[4]?(l[1]="@supports (".concat(l[4],") {").concat(l[1],"}"),l[4]=i):l[4]="".concat(i)),o.push(l))}},o}},81:t=>{"use strict";t.exports=function(t){return t[1]}},379:t=>{"use strict";var o=[];function n(t){for(var n=-1,e=0;e<o.length;e++)if(o[e].identifier===t){n=e;break}return n}function e(t,e){for(var s={},a=[],r=0;r<t.length;r++){var c=t[r],p=e.base?c[0]+e.base:c[0],l=s[p]||0,d="".concat(p," ").concat(l);s[p]=l+1;var u=n(d),f={css:c[1],media:c[2],sourceMap:c[3],supports:c[4],layer:c[5]};if(-1!==u)o[u].references++,o[u].updater(f);else{var h=i(f,e);e.byIndex=r,o.splice(r,0,{identifier:d,updater:h,references:1})}a.push(d)}return a}function i(t,o){var n=o.domAPI(o);return n.update(t),function(o){if(o){if(o.css===t.css&&o.media===t.media&&o.sourceMap===t.sourceMap&&o.supports===t.supports&&o.layer===t.layer)return;n.update(t=o)}else n.remove()}}t.exports=function(t,i){var s=e(t=t||[],i=i||{});return function(t){t=t||[];for(var a=0;a<s.length;a++){var r=n(s[a]);o[r].references--}for(var c=e(t,i),p=0;p<s.length;p++){var l=n(s[p]);0===o[l].references&&(o[l].updater(),o.splice(l,1))}s=c}}},569:t=>{"use strict";var o={};t.exports=function(t,n){var e=function(t){if(void 0===o[t]){var n=document.querySelector(t);if(window.HTMLIFrameElement&&n instanceof window.HTMLIFrameElement)try{n=n.contentDocument.head}catch(t){n=null}o[t]=n}return o[t]}(t);if(!e)throw new Error("Couldn't find a style target. This probably means that the value for the 'insert' parameter is invalid.");e.appendChild(n)}},216:t=>{"use strict";t.exports=function(t){var o=document.createElement("style");return t.setAttributes(o,t.attributes),t.insert(o,t.options),o}},565:(t,o,n)=>{"use strict";t.exports=function(t){var o=n.nc;o&&t.setAttribute("nonce",o)}},795:t=>{"use strict";t.exports=function(t){var o=t.insertStyleElement(t);return{update:function(n){!function(t,o,n){var e="";n.supports&&(e+="@supports (".concat(n.supports,") {")),n.media&&(e+="@media ".concat(n.media," {"));var i=void 0!==n.layer;i&&(e+="@layer".concat(n.layer.length>0?" ".concat(n.layer):""," {")),e+=n.css,i&&(e+="}"),n.media&&(e+="}"),n.supports&&(e+="}");var s=n.sourceMap;s&&"undefined"!=typeof btoa&&(e+="\n/*# sourceMappingURL=data:application/json;base64,".concat(btoa(unescape(encodeURIComponent(JSON.stringify(s))))," */")),o.styleTagTransform(e,t,o.options)}(o,t,n)},remove:function(){!function(t){if(null===t.parentNode)return!1;t.parentNode.removeChild(t)}(o)}}}},589:t=>{"use strict";t.exports=function(t,o){if(o.styleSheet)o.styleSheet.cssText=t;else{for(;o.firstChild;)o.removeChild(o.firstChild);o.appendChild(document.createTextNode(t))}}},588:function(t){var o;o=function(t){var o=function(t){return new o.lib.init(t)};function n(t,o){return o.offset[t]?isNaN(o.offset[t])?o.offset[t]:o.offset[t]+"px":"0px"}function e(t,o){return!(!t||"string"!=typeof o||!(t.className&&t.className.trim().split(/\s+/gi).indexOf(o)>-1))}return o.defaults={oldestFirst:!0,text:"Toastify is awesome!",node:void 0,duration:300,selector:void 0,callback:function(){},destination:void 0,newWindow:!1,close:!1,gravity:"toastify-top",positionLeft:!1,position:"",backgroundColor:"",avatar:"",className:"",stopOnFocus:!0,onClick:function(){},offset:{x:0,y:0},escapeMarkup:!0,ariaLive:"polite",style:{background:""}},o.lib=o.prototype={toastify:"1.12.0",constructor:o,init:function(t){return t||(t={}),this.options={},this.toastElement=null,this.options.text=t.text||o.defaults.text,this.options.node=t.node||o.defaults.node,this.options.duration=0===t.duration?0:t.duration||o.defaults.duration,this.options.selector=t.selector||o.defaults.selector,this.options.callback=t.callback||o.defaults.callback,this.options.destination=t.destination||o.defaults.destination,this.options.newWindow=t.newWindow||o.defaults.newWindow,this.options.close=t.close||o.defaults.close,this.options.gravity="bottom"===t.gravity?"toastify-bottom":o.defaults.gravity,this.options.positionLeft=t.positionLeft||o.defaults.positionLeft,this.options.position=t.position||o.defaults.position,this.options.backgroundColor=t.backgroundColor||o.defaults.backgroundColor,this.options.avatar=t.avatar||o.defaults.avatar,this.options.className=t.className||o.defaults.className,this.options.stopOnFocus=void 0===t.stopOnFocus?o.defaults.stopOnFocus:t.stopOnFocus,this.options.onClick=t.onClick||o.defaults.onClick,this.options.offset=t.offset||o.defaults.offset,this.options.escapeMarkup=void 0!==t.escapeMarkup?t.escapeMarkup:o.defaults.escapeMarkup,this.options.ariaLive=t.ariaLive||o.defaults.ariaLive,this.options.style=t.style||o.defaults.style,t.backgroundColor&&(this.options.style.background=t.backgroundColor),this},buildToast:function(){if(!this.options)throw"Toastify is not initialized";var t=document.createElement("div");for(var o in t.className="toastify on "+this.options.className,this.options.position?t.className+=" toastify-"+this.options.position:!0===this.options.positionLeft?(t.className+=" toastify-left",console.warn("Property `positionLeft` will be depreciated in further versions. Please use `position` instead.")):t.className+=" toastify-right",t.className+=" "+this.options.gravity,this.options.backgroundColor&&console.warn('DEPRECATION NOTICE: "backgroundColor" is being deprecated. Please use the "style.background" property.'),this.options.style)t.style[o]=this.options.style[o];if(this.options.ariaLive&&t.setAttribute("aria-live",this.options.ariaLive),this.options.node&&this.options.node.nodeType===Node.ELEMENT_NODE)t.appendChild(this.options.node);else if(this.options.escapeMarkup?t.innerText=this.options.text:t.innerHTML=this.options.text,""!==this.options.avatar){var e=document.createElement("img");e.src=this.options.avatar,e.className="toastify-avatar","left"==this.options.position||!0===this.options.positionLeft?t.appendChild(e):t.insertAdjacentElement("afterbegin",e)}if(!0===this.options.close){var i=document.createElement("button");i.type="button",i.setAttribute("aria-label","Close"),i.className="toast-close",i.innerHTML="&#10006;",i.addEventListener("click",function(t){t.stopPropagation(),this.removeElement(this.toastElement),window.clearTimeout(this.toastElement.timeOutValue)}.bind(this));var s=window.innerWidth>0?window.innerWidth:screen.width;("left"==this.options.position||!0===this.options.positionLeft)&&s>360?t.insertAdjacentElement("afterbegin",i):t.appendChild(i)}if(this.options.stopOnFocus&&this.options.duration>0){var a=this;t.addEventListener("mouseover",(function(o){window.clearTimeout(t.timeOutValue)})),t.addEventListener("mouseleave",(function(){t.timeOutValue=window.setTimeout((function(){a.removeElement(t)}),a.options.duration)}))}if(void 0!==this.options.destination&&t.addEventListener("click",function(t){t.stopPropagation(),!0===this.options.newWindow?window.open(this.options.destination,"_blank"):window.location=this.options.destination}.bind(this)),"function"==typeof this.options.onClick&&void 0===this.options.destination&&t.addEventListener("click",function(t){t.stopPropagation(),this.options.onClick()}.bind(this)),"object"==typeof this.options.offset){var r=n("x",this.options),c=n("y",this.options),p="left"==this.options.position?r:"-"+r,l="toastify-top"==this.options.gravity?c:"-"+c;t.style.transform="translate("+p+","+l+')'}return t}, showToast:function() { var t; if (this.toastElement = this.buildToast(), !(t = "string" == typeof this.options.selector ? document.getElementById(this.options.selector) : this.options.selector instanceof HTMLElement || "undefined" != typeof ShadowRoot && this.options.selector instanceof ShadowRoot ? this.options.selector : document.body))throw"Root element is not defined"; var n = o.defaults.oldestFirst ? t.firstChild : t.lastChild; return t.insertBefore(this.toastElement, n), o.reposition(), this.options.duration > 0 && (this.toastElement.timeOutValue = window.setTimeout(function() { this.removeElement(this.toastElement) }.bind(this), this.options.duration)), this }, hideToast : function() { this.toastElement.timeOutValue&& clearTimeout(this.toastElement.timeOutValue), this.removeElement(this.toastElement) }, removeElement : function(t) { t.className = t.className.replace(" on", ""), window.setTimeout(function() { this.options.node&& this.options.node.parentNode&& this.options.node.parentNode.removeChild(this.options.node), t.parentNode&& t.parentNode.removeChild(t), this.options.callback.call(t), o.reposition() }.bind(this), 400) }}, o.reposition = function() { for (var t, o = { top:15,bottom : 15 }, n = { top:15,bottom : 15 }, i = { top:15,bottom : 15 }, s = document.getElementsByClassName("toastify"), a = 0; a < s.length; a++) { t = !0 === e(s[a], "toastify-top") ? "toastify-top" : "toastify-bottom"; var r = s[a].offsetHeight; t = t.substr(9, t.length - 1), (window.innerWidth > 0 ? window.innerWidth : screen.width) <= 360 ? (s[a].style[t] = i[t] + "px", i[t] += r + 15) : !0 === e(s[a], "toastify-left") ? (s[a].style[t] = o[t] + "px", o[t] += r + 15) : (s[a].style[t] = n[t] + "px", n[t] += r + 15) }return this }, o.lib.init.prototype = o.lib, o}, t.exports ? t.exports = o() : this.Toastify = o()}}, o = {}; function n(e) { var i = o[e]; if (void 0 !== i)return i.exports; var s = o[e] = { id:e,exports : {} }; return t[e].call(s.exports, s, s.exports, n), s.exports }n.n = t => {var o = t && t.__esModule ? () => t.default:() => t; return n.d(o, { a:o }), o}, n.d = (t, o) => {for (var e in o)n.o(o, e) && !n.o(t, e) && Object.defineProperty(t, e, { enumerable:!0,get : o[e] })}, n.o = (t, o) => Object.prototype.hasOwnProperty.call(t, o), n.nc = void 0, (() => {"use strict"; var t = n(588), o = n.n(t), e = n(379), i = n.n(e), s = n(795), a = n.n(s), r = n(569), c = n.n(r), p = n(565), l = n.n(p), d = n(216), u = n.n(d), f = n(589), h = n.n(f), m = n(235), v = {}; v.styleTagTransform = h(), v.setAttributes = l(), v.insert = c().bind(null, "head"), v.domAPI = a(), v.insertStyleElement = u(), i()(m.Z, v), m.Z&& m.Z.locals&& m.Z.locals, window.customToast = function(t) { o()({ text:t,duration : 3e3,destination : "https://github.com/apvarun/toastify-js",newWindow : !0,close : !1,gravity : "top",position : "center",stopOnFocus : !1,style : {background:'linear-gradient(to right, #f1cede, #fc70b0)',padding : "4px 10px"},onClick : function() {} }).showToast() }})()})();
)";


string reconnectWebsocket = R"(
	var ReconnectingWebSocket = (function () {
        'use strict';
        var extendStatics = function(d, b) {
            extendStatics = Object.setPrototypeOf ||
                ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
                function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
            return extendStatics(d, b);
        };

        function __extends(d, b) {
            extendStatics(d, b);
            function __() { this.constructor = d; }
            d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
        }

        function __values(o) {
            var m = typeof Symbol === "function" && o[Symbol.iterator], i = 0;
            if (m) return m.call(o);
            return {
                next: function () {
                    if (o && i >= o.length) o = void 0;
                    return { value: o && o[i++], done: !o };
                }
            };
        }

        function __read(o, n) {
            var m = typeof Symbol === "function" && o[Symbol.iterator];
            if (!m) return o;
            var i = m.call(o), r, ar = [], e;
            try {
                while ((n === void 0 || n-- > 0) && !(r = i.next()).done) ar.push(r.value);
            }
            catch (error) { e = { error: error }; }
            finally {
                try {
                    if (r && !r.done && (m = i["return"])) m.call(i);
                }
                finally { if (e) throw e.error; }
            }
            return ar;
        }

        function __spread() {
            for (var ar = [], i = 0; i < arguments.length; i++)
                ar = ar.concat(__read(arguments[i]));
            return ar;
        }

        var Event = /** @class */ (function () {
            function Event(type, target) {
                this.target = target;
                this.type = type;
            }
            return Event;
        }());
        var ErrorEvent = /** @class */ (function (_super) {
            __extends(ErrorEvent, _super);
            function ErrorEvent(error, target) {
                var _this = _super.call(this, 'error', target) || this;
                _this.message = error.message;
                _this.error = error;
                return _this;
            }
            return ErrorEvent;
        }(Event));
        var CloseEvent = /** @class */ (function (_super) {
            __extends(CloseEvent, _super);
            function CloseEvent(code, reason, target) {
                if (code === void 0) { code = 1000; }
                if (reason === void 0) { reason = ''; }
                var _this = _super.call(this, 'close', target) || this;
                _this.wasClean = true;
                _this.code = code;
                _this.reason = reason;
                return _this;
            }
            return CloseEvent;
        }(Event));

        /*!
         * Reconnecting WebSocket
         * by Pedro Ladaria <pedro.ladaria@gmail.com>
         * https://github.com/pladaria/reconnecting-websocket
         * License MIT
         */
        var getGlobalWebSocket = function () {
            if (typeof WebSocket !== 'undefined') {
                // @ts-ignore
                return WebSocket;
            }
        };
        /**
         * Returns true if given argument looks like a WebSocket class
         */
        var isWebSocket = function (w) { return typeof w !== 'undefined' && !!w && w.CLOSING === 2; };
        var DEFAULT = {
            maxReconnectionDelay: 10000,
            minReconnectionDelay: 1000 + Math.random() * 4000,
            minUptime: 5000,
            reconnectionDelayGrowFactor: 1.3,
            connectionTimeout: 4000,
            maxRetries: Infinity,
            maxEnqueuedMessages: Infinity,
            startClosed: false,
            debug: false,
        };
        var ReconnectingWebSocket = /** @class */ (function () {
            function ReconnectingWebSocket(url, protocols, options) {
                var _this = this;
                if (options === void 0) { options = {}; }
                this._listeners = {
                    error: [],
                    message: [],
                    open: [],
                    close: [],
                };
                this._retryCount = -1;
                this._shouldReconnect = true;
                this._connectLock = false;
                this._binaryType = 'blob';
                this._closeCalled = false;
                this._messageQueue = [];
                /**
                 * An event listener to be called when the WebSocket connection's readyState changes to CLOSED
                 */
                this.onclose = null;
                /**
                 * An event listener to be called when an error occurs
                 */
                this.onerror = null;
                /**
                 * An event listener to be called when a message is received from the server
                 */
                this.onmessage = null;
                /**
                 * An event listener to be called when the WebSocket connection's readyState changes to OPEN;
                 * this indicates that the connection is ready to send and receive data
                 */
                this.onopen = null;
                this._handleOpen = function (event) {
                    _this._debug('open event');
                    var _a = _this._options.minUptime, minUptime = _a === void 0 ? DEFAULT.minUptime : _a;
                    clearTimeout(_this._connectTimeout);
                    _this._uptimeTimeout = setTimeout(function () { return _this._acceptOpen(); }, minUptime);
                    _this._ws.binaryType = _this._binaryType;
                    // send enqueued messages (messages sent before websocket open event)
                    _this._messageQueue.forEach(function (message) { return _this._ws.send(message); });
                    _this._messageQueue = [];
                    if (_this.onopen) {
                        _this.onopen(event);
                    }
                    _this._listeners.open.forEach(function (listener) { return _this._callEventListener(event, listener); });
                };
                this._handleMessage = function (event) {
                    _this._debug('message event');
                    if (_this.onmessage) {
                        _this.onmessage(event);
                    }
                    _this._listeners.message.forEach(function (listener) { return _this._callEventListener(event, listener); });
                };
                this._handleError = function (event) {
                    _this._debug('error event', event.message);
                    _this._disconnect(undefined, event.message === 'TIMEOUT' ? 'timeout' : undefined);
                    if (_this.onerror) {
                        _this.onerror(event);
                    }
                    _this._debug('exec error listeners');
                    _this._listeners.error.forEach(function (listener) { return _this._callEventListener(event, listener); });
                    _this._connect();
                };
                this._handleClose = function (event) {
                    _this._debug('close event');
                    _this._clearTimeouts();
                    if (_this._shouldReconnect) {
                        _this._connect();
                    }
                    if (_this.onclose) {
                        _this.onclose(event);
                    }
                    _this._listeners.close.forEach(function (listener) { return _this._callEventListener(event, listener); });
                };
                this._url = url;
                this._protocols = protocols;
                this._options = options;
                if (this._options.startClosed) {
                    this._shouldReconnect = false;
                }
                this._connect();
            }
            Object.defineProperty(ReconnectingWebSocket, "CONNECTING", {
                get: function () {
                    return 0;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket, "OPEN", {
                get: function () {
                    return 1;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket, "CLOSING", {
                get: function () {
                    return 2;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket, "CLOSED", {
                get: function () {
                    return 3;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "CONNECTING", {
                get: function () {
                    return ReconnectingWebSocket.CONNECTING;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "OPEN", {
                get: function () {
                    return ReconnectingWebSocket.OPEN;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "CLOSING", {
                get: function () {
                    return ReconnectingWebSocket.CLOSING;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "CLOSED", {
                get: function () {
                    return ReconnectingWebSocket.CLOSED;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "binaryType", {
                get: function () {
                    return this._ws ? this._ws.binaryType : this._binaryType;
                },
                set: function (value) {
                    this._binaryType = value;
                    if (this._ws) {
                        this._ws.binaryType = value;
                    }
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "retryCount", {
                /**
                 * Returns the number or connection retries
                 */
                get: function () {
                    return Math.max(this._retryCount, 0);
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "bufferedAmount", {
                /**
                 * The number of bytes of data that have been queued using calls to send() but not yet
                 * transmitted to the network. This value resets to zero once all queued data has been sent.
                 * This value does not reset to zero when the connection is closed; if you keep calling send(),
                 * this will continue to climb. Read only
                 */
                get: function () {
                    var bytes = this._messageQueue.reduce(function (acc, message) {
                        if (typeof message === 'string') {
                            acc += message.length; // not byte size
                        }
                        else if (message instanceof Blob) {
                            acc += message.size;
                        }
                        else {
                            acc += message.byteLength;
                        }
                        return acc;
                    }, 0);
                    return bytes + (this._ws ? this._ws.bufferedAmount : 0);
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "extensions", {
                /**
                 * The extensions selected by the server. This is currently only the empty string or a list of
                 * extensions as negotiated by the connection
                 */
                get: function () {
                    return this._ws ? this._ws.extensions : '';
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "protocol", {
                /**
                 * A string indicating the name of the sub-protocol the server selected;
                 * this will be one of the strings specified in the protocols parameter when creating the
                 * WebSocket object
                 */
                get: function () {
                    return this._ws ? this._ws.protocol : '';
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "readyState", {
                /**
                 * The current state of the connection; this is one of the Ready state constants
                 */
                get: function () {
                    if (this._ws) {
                        return this._ws.readyState;
                    }
                    return this._options.startClosed
                        ? ReconnectingWebSocket.CLOSED
                        : ReconnectingWebSocket.CONNECTING;
                },
                enumerable: true,
                configurable: true
            });
            Object.defineProperty(ReconnectingWebSocket.prototype, "url", {
                /**
                 * The URL as resolved by the constructor
                 */
                get: function () {
                    return this._ws ? this._ws.url : '';
                },
                enumerable: true,
                configurable: true
            });
            /**
             * Closes the WebSocket connection or connection attempt, if any. If the connection is already
             * CLOSED, this method does nothing
             */
            ReconnectingWebSocket.prototype.close = function (code, reason) {
                if (code === void 0) { code = 1000; }
                this._closeCalled = true;
                this._shouldReconnect = false;
                this._clearTimeouts();
                if (!this._ws) {
                    this._debug('close enqueued: no ws instance');
                    return;
                }
                if (this._ws.readyState === this.CLOSED) {
                    this._debug('close: already closed');
                    return;
                }
                this._ws.close(code, reason);
            };
            /**
             * Closes the WebSocket connection or connection attempt and connects again.
             * Resets retry counter;
             */

            )"
    R"(
            ReconnectingWebSocket.prototype.reconnect = function (code, reason) {
                this._shouldReconnect = true;
                this._closeCalled = false;
                this._retryCount = -1;
                if (!this._ws || this._ws.readyState === this.CLOSED) {
                    this._connect();
                }
                else {
                    this._disconnect(code, reason);
                    this._connect();
                }
            };
            /**
             * Enqueue specified data to be transmitted to the server over the WebSocket connection
             */
            ReconnectingWebSocket.prototype.send = function (data) {
                if (this._ws && this._ws.readyState === this.OPEN) {
                    this._debug('send', data);
                    this._ws.send(data);
                }
                else {
                    var _a = this._options.maxEnqueuedMessages, maxEnqueuedMessages = _a === void 0 ? DEFAULT.maxEnqueuedMessages : _a;
                    if (this._messageQueue.length < maxEnqueuedMessages) {
                        this._debug('enqueue', data);
                        this._messageQueue.push(data);
                    }
                }
            };
            /**
             * Register an event handler of a specific event type
             */
            ReconnectingWebSocket.prototype.addEventListener = function (type, listener) {
                if (this._listeners[type]) {
                    // @ts-ignore
                    this._listeners[type].push(listener);
                }
            };
            ReconnectingWebSocket.prototype.dispatchEvent = function (event) {
                var e_1, _a;
                var listeners = this._listeners[event.type];
                if (listeners) {
                    try {
                        for (var listeners_1 = __values(listeners), listeners_1_1 = listeners_1.next(); !listeners_1_1.done; listeners_1_1 = listeners_1.next()) {
                            var listener = listeners_1_1.value;
                            this._callEventListener(event, listener);
                        }
                    }
                    catch (e_1_1) { e_1 = { error: e_1_1 }; }
                    finally {
                        try {
                            if (listeners_1_1 && !listeners_1_1.done && (_a = listeners_1.return)) _a.call(listeners_1);
                        }
                        finally { if (e_1) throw e_1.error; }
                    }
                }
                return true;
            };
            /**
             * Removes an event listener
             */
            ReconnectingWebSocket.prototype.removeEventListener = function (type, listener) {
                if (this._listeners[type]) {
                    // @ts-ignore
                    this._listeners[type] = this._listeners[type].filter(function (l) { return l !== listener; });
                }
            };
            ReconnectingWebSocket.prototype._debug = function () {
                var args = [];
                for (var _i = 0; _i < arguments.length; _i++) {
                    args[_i] = arguments[_i];
                }
                if (this._options.debug) {
                    // not using spread because compiled version uses Symbols
                    // tslint:disable-next-line
                    console.log.apply(console, __spread(['RWS>'], args));
                }
            };
            ReconnectingWebSocket.prototype._getNextDelay = function () {
                var _a = this._options, _b = _a.reconnectionDelayGrowFactor, reconnectionDelayGrowFactor = _b === void 0 ? DEFAULT.reconnectionDelayGrowFactor : _b, _c = _a.minReconnectionDelay, minReconnectionDelay = _c === void 0 ? DEFAULT.minReconnectionDelay : _c, _d = _a.maxReconnectionDelay, maxReconnectionDelay = _d === void 0 ? DEFAULT.maxReconnectionDelay : _d;
                var delay = 0;
                if (this._retryCount > 0) {
                    delay =
                        minReconnectionDelay * Math.pow(reconnectionDelayGrowFactor, this._retryCount - 1);
                    if (delay > maxReconnectionDelay) {
                        delay = maxReconnectionDelay;
                    }
                }
                this._debug('next delay', delay);
                return delay;
            };
            ReconnectingWebSocket.prototype._wait = function () {
                var _this = this;
                return new Promise(function (resolve) {
                    setTimeout(resolve, _this._getNextDelay());
                });
            };
            ReconnectingWebSocket.prototype._getNextUrl = function (urlProvider) {
                if (typeof urlProvider === 'string') {
                    return Promise.resolve(urlProvider);
                }
                if (typeof urlProvider === 'function') {
                    var url = urlProvider();
                    if (typeof url === 'string') {
                        return Promise.resolve(url);
                    }
                    if (!!url.then) {
                        return url;
                    }
                }
                throw Error('Invalid URL');
            };
            ReconnectingWebSocket.prototype._connect = function () {
                var _this = this;
                if (this._connectLock || !this._shouldReconnect) {
                    return;
                }
                this._connectLock = true;
                var _a = this._options, _b = _a.maxRetries, maxRetries = _b === void 0 ? DEFAULT.maxRetries : _b, _c = _a.connectionTimeout, connectionTimeout = _c === void 0 ? DEFAULT.connectionTimeout : _c, _d = _a.WebSocket, WebSocket = _d === void 0 ? getGlobalWebSocket() : _d;
                if (this._retryCount >= maxRetries) {
                    this._debug('max retries reached', this._retryCount, '>=', maxRetries);
                    return;
                }
                this._retryCount++;
                this._debug('connect', this._retryCount);
                this._removeListeners();
                if (!isWebSocket(WebSocket)) {
                    throw Error('No valid WebSocket class provided');
                }
                this._wait()
                    .then(function () { return _this._getNextUrl(_this._url); })
                    .then(function (url) {
                    // close could be called before creating the ws
                    if (_this._closeCalled) {
                        return;
                    }
                    _this._debug('connect', { url: url, protocols: _this._protocols });
                    _this._ws = _this._protocols
                        ? new WebSocket(url, _this._protocols)
                        : new WebSocket(url);
                    _this._ws.binaryType = _this._binaryType;
                    _this._connectLock = false;
                    _this._addListeners();
                    _this._connectTimeout = setTimeout(function () { return _this._handleTimeout(); }, connectionTimeout);
                });
            };
            ReconnectingWebSocket.prototype._handleTimeout = function () {
                this._debug('timeout event');
                this._handleError(new ErrorEvent(Error('TIMEOUT'), this));
            };
            ReconnectingWebSocket.prototype._disconnect = function (code, reason) {
                if (code === void 0) { code = 1000; }
                this._clearTimeouts();
                if (!this._ws) {
                    return;
                }
                this._removeListeners();
                try {
                    this._ws.close(code, reason);
                    this._handleClose(new CloseEvent(code, reason, this));
                }
                catch (error) {
                    // ignore
                }
            };
            ReconnectingWebSocket.prototype._acceptOpen = function () {
                this._debug('accept open');
                this._retryCount = 0;
            };
            ReconnectingWebSocket.prototype._callEventListener = function (event, listener) {
                if ('handleEvent' in listener) {
                    // @ts-ignore
                    listener.handleEvent(event);
                }
                else {
                    // @ts-ignore
                    listener(event);
                }
            };
            ReconnectingWebSocket.prototype._removeListeners = function () {
                if (!this._ws) {
                    return;
                }
                this._debug('removeListeners');
                this._ws.removeEventListener('open', this._handleOpen);
                this._ws.removeEventListener('close', this._handleClose);
                this._ws.removeEventListener('message', this._handleMessage);
                // @ts-ignore
                this._ws.removeEventListener('error', this._handleError);
            };
            ReconnectingWebSocket.prototype._addListeners = function () {
                if (!this._ws) {
                    return;
                }
                this._debug('addListeners');
                this._ws.addEventListener('open', this._handleOpen);
                this._ws.addEventListener('close', this._handleClose);
                this._ws.addEventListener('message', this._handleMessage);
                // @ts-ignore
                this._ws.addEventListener('error', this._handleError);
            };
            ReconnectingWebSocket.prototype._clearTimeouts = function () {
                clearTimeout(this._connectTimeout);
                clearTimeout(this._uptimeTimeout);
            };
            return ReconnectingWebSocket;
        }());

        return ReconnectingWebSocket;

    }());
)";