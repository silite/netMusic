import Toastify from 'toastify-js'
import 'toastify-js/src/toastify.css'

window.customToast = function(text) {
  Toastify({
    text,
    duration: 3000,
    destination: "https://github.com/apvarun/toastify-js",
    newWindow: true,
    close: false,
    gravity: "top", // `top` or `bottom`
    position: "center", // `left`, `center` or `right`
    stopOnFocus: false, // Prevents dismissing of toast on hover
    style: {
      background: "linear-gradient(to right, #f1cede, #fc70b0)",
      padding: '4px 10px'
    },
    onClick: function(){} // Callback after click
  }).showToast();
}
