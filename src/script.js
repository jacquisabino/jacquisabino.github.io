
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

function openProject(newTxt){
    const oldHeader = document.getElementsByClassName("headerText");
    oldHeader.innerHTML = newTxt;
}

function openProjectOne(){
    window.location.href = "../currentProjects/generalProjects.html";
}

function projectButtonClicked(){
    window.location.href = "../projects.html"
}