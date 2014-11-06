var Alloy = require("alloy"), _ = Alloy._, Backbone = Alloy.Backbone;

var pushnotifications = require("com.arellomobile.push");

Ti.API.info("module is => " + pushnotifications);

pushnotifications.pushNotificationsRegister("387050748417", "A0443-C41F6", {
    success: function(e) {
        Ti.API.info("TITAIUM!!! JS registration success event: " + e.registrationId);
    },
    error: function(e) {
        Ti.API.error("TITAIUM!!! Error during registration: " + e.error);
    },
    callback: function(e) {
        Ti.API.info("TITAIUM!!! JS message event: " + JSON.stringify(e.data));
    }
});

Alloy.createController("index");