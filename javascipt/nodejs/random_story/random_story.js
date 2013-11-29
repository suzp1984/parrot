var fs = require('fs');
var request = require('request');
var htmlparser = require('htmlparser');

var tasks = [
    function () {
        var configFilename = './rss_feeds.txt';
        fs.exists(configFilename, function(exists) {
            if (!exists) {
                next('Create a list of RSS feeds in the file ./rss_feeds.txt.');
                
            } else {
                next(false, configFilename);
            }
        });
    },
    function (configFilename) {
        fs.readFile(configFilename, function(err, feedList) {
            if (err) {
                next(err.message);
            } else {
                feedList = feedList.toString()
                .replace(/^\s+|\s+$/g, '')
                .split("\n");
                var random = Math.floor(Math.random() * feedList.length);
                next(false, feedList[random]);
            }
        });
    },
    function (feedUrl) {
        console.log("Requst feedUrl " + feedUrl);
        request({uri: feedUrl}, function(err, response, body) {
            if (err) {
                next(err.message);
            } else if (response.statusCode == 200) {
                next(false, body);
            } else {
                next('Abnormal request status code.');
            }
        });
    },
    function (rss) {
        console.log("before complete rss body.");
        var handler = new htmlparser.RssHandler();
        var parser = new htmlparser.Parser(handler);
        parser.parseComplete(rss);
        
        if (handler.dom.items.length) {
            var item;
            while(item = handler.dom.items.shift()) {
                console.log(item.title);
                console.log(item.link);
            }
        } else {
            next('No Rss items found.');
        }
    }];

function next(err, result) {
    if (err) throw new Error(err);
    var currentTask = tasks.shift();

    if (currentTask) {
        currentTask(result);
    }
}

next();
