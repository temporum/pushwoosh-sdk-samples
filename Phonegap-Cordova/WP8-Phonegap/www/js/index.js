/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
var app = {
    // Application Constructor
    initialize: function () {
        this.bindEvents();
    },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function () {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicity call 'app.receivedEvent(...);'
    onDeviceReady: function () {
        app.receivedEvent('deviceready');
        initPushwoosh();
    },
    // Update DOM on a Received Event
    receivedEvent: function (id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');

        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');

        console.log('Received Event: ' + id);
    }
};

function initPushwoosh() {
    var pushNotification = window.plugins.pushNotification;

    //set push notification callback before we initialize the plugin
    document.addEventListener('push-notification', function (event) {
        //get the notification payload
        var notification = event.notification;

        //display alert to the user for example
        alert(JSON.stringify(notification));
    });

    //initialize the plugin
    pushNotification.onDeviceReady({ appid: "3A43A-A3EAB", serviceName: "" });

    //register for pushes
    pushNotification.registerDevice(
        function (status) {
            var deviceToken = status;
            console.warn('registerDevice: ' + deviceToken);
            alert("push token is " + deviceToken);
            pushwooshInitialized();
        },
        function (status) {
            console.warn('failed to register : ' + JSON.stringify(status));
            alert(JSON.stringify(['failed to register ', status]));
        }
    );
}

function pushwooshInitialized()
{
    var pushNotification = window.plugins.pushNotification;

    //there is a MASSIVE bug in cordova, still present at 3.6.0-dev :(
    //https://github.com/apache/cordova-wp8/commit/fb9cf558c65bd0e580915f2a6d7ab96986e85965
    //do not call several plugin command at once!

    //if you need push token at a later time you can always get it from Pushwoosh plugin
    /*pushNotification.getPushToken(
        function (token) {
            alert('push token: ' + token);
        }
    );
*/

    //and HWID if you want to communicate with Pushwoosh API
    /*pushNotification.getPushwooshHWID(
        function (token) {
            alert('Pushwoosh HWID: ' + token);
        }
    );
*/
    //settings tags
    pushNotification.setTags({ tagName: "tagValue", intTagName: 10 },
        function (status) {
            alert('setTags success: ' + JSON.stringify(status));
        },
        function (status) {
            console.warn('setTags failed');
        }
    );

    /*
    pushNotification.getTags(
        function (status) {
            alert('getTags success: ' + JSON.stringify(status));
        },
        function (status) {
            console.warn('getTags failed');
        }
    );
    */
}