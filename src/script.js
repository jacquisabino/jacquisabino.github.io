
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

function openProjectOne(){
    window.location.href = "../currentProjects/generalProjects.html";
}

function projectButtonClicked(){
    window.location.href = "../projects.html"
}