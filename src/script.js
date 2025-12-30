
function displayEducation(){
    window.location.href = "education.html";
}

function displayWork(){
    window.location.href = "work.html";
}

function displayContact(){
    window.location.href = "contact.html";
}

function displayProjects(){
    window.location.href = "projects.html";
}

function buttonClicked(){
    window.location.href = "index.html";
}

/* need to generalize function for opening projects  */

function openProject(val){
    const oldHeader = document.getElementsByClassName("headerText");
    if(val == 1){
        oldHeader.innerHTML = "Cache Simulator";
    } else if(val == 2){
        oldHeader.innerHTML = "Public Works Database";
    } else {
        oldHeader.innerHTML = "Code Generator";
    }
}


function projectButtonClicked(){
    window.location.href = "../projects.html"
}

/* Individula project displays: might want to gneralize but not sure  */
function openProjectOne(){
    window.location.href = "../currentProjects/cacheSimProj.html";
}

function openProjectTwo(){
    window.location.href = "../currentProjects/pubWorksProj.html";
}

function openProjectThree(){
    window.location.href = "../currentProjects/codeGenProj.html";
}
