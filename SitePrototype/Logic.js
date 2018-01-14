var folderNames = document.getElementsByClassName('folder-name');
var lastOpenedFolder = null;

for (var i = 0; i < folderNames.length; ++i) {
    folderNames[i].addEventListener('click', function() {

        this.parentElement.lastElementChild.style.height = 'auto';
        if (lastOpenedFolder != null) {
            while (lastOpenedFolder.className == 'folder') {
                lastOpenedFolder.lastElementChild.style.height = '0px';

                if (lastOpenedFolder == this.parentElement) {
                    lastOpenedFolder = lastOpenedFolder.parentElement.parentElement;
                    if (lastOpenedFolder.className != 'folder') {
                        lastOpenedFolder = null;
                    }
                    return;
                }
                lastOpenedFolder = lastOpenedFolder.parentElement.parentElement;
            }
            var currentOpenedFolder = this.parentElement;
            while (currentOpenedFolder.className == 'folder') {
                currentOpenedFolder.lastElementChild.style.height = 'auto';
                currentOpenedFolder = currentOpenedFolder.parentElement.parentElement;
            }
        }
        lastOpenedFolder = this.parentElement;
    });
}

var formNames = document.getElementsByClassName('form-name');

for (var i = 0; i < formNames.length; ++i) {
    formNames[i].addEventListener('click', function() {
        
        var id = this.id;
        // TODO request here
    });
}