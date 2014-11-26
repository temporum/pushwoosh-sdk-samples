cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/com.blackberry.app/www/client.js",
        "id": "com.blackberry.app.client",
        "clobbers": [
            "blackberry.app"
        ]
    },
    {
        "file": "plugins/com.blackberry.identity/www/client.js",
        "id": "com.blackberry.identity.client",
        "clobbers": [
            "blackberry.identity"
        ]
    },
    {
        "file": "plugins/com.blackberry.invoked/www/client.js",
        "id": "com.blackberry.invoked.client",
        "clobbers": [
            "blackberry.invoked"
        ]
    },
    {
        "file": "plugins/com.blackberry.push/www/client.js",
        "id": "com.blackberry.push.client",
        "clobbers": [
            "blackberry.push"
        ]
    },
    {
        "file": "plugins/com.blackberry.notification/www/client.js",
        "id": "com.blackberry.notification.client",
        "clobbers": [
            "blackberry.notification"
        ]
    },
    {
        "file": "plugins/com.blackberry.ui.dialog/www/client.js",
        "id": "com.blackberry.ui.dialog.client",
        "clobbers": [
            "blackberry.ui.dialog"
        ]
    },
    {
        "file": "plugins/com.blackberry.ui.toast/www/client.js",
        "id": "com.blackberry.ui.toast.client",
        "clobbers": [
            "blackberry.ui.toast"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "com.blackberry.app": "1.0.0",
    "com.blackberry.identity": "1.0.0",
    "com.blackberry.invoked": "1.0.0",
    "com.blackberry.push": "1.0.0",
    "com.blackberry.notification": "1.0.0",
    "com.blackberry.ui.dialog": "1.0.0",
    "com.blackberry.ui.toast": "1.0.0",
    "com.blackberry.utils": "1.0.0"
}
// BOTTOM OF METADATA
});