var fs = require('fs');
var events = require('events');
var util = require('util');
var watchDir = './watch';
var processedDir = './done';

function FsWatcher(watchDir, processedDir) {
    this.watchDir = watchDir;
    this.processedDir = processedDir;
}

util.inherits(FsWatcher, events.EventEmitter);

FsWatcher.prototype.watch = function() {
    var watcher = this;
    fs.readdir(this.watchDir, function(err, files) {
        if (err) throw err;
        for(var index in files) {
            watcher.emit('process', files[index]);
        }
    });
};

FsWatcher.prototype.start = function() {
    var watcher = this;
    fs.watchFile(watchDir, function() {
        watcher.watch();
    });
};

var watcher = new FsWatcher(watchDir, processedDir);

watcher.on('process', function process (file) {
    console.log("start process func");
    var watchFile = this.watchDir + '/' + file;
    var processedFile = this.processedDir + '/' + file.toLowerCase();
    
    fs.rename(watchFile, processedFile, function(err) {
        if (err) throw err;
    });
});

watcher.start()
