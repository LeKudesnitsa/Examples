//'use strict';

//---HEAD------------------


var optionsDefault = Object.assign({}, options);


//---LISTENERS---
//FUNCTION WITHOUT BRACKETS!!!!! or contained in anonymious func: function(){ .. } 
//tabs sorting in order of creating, not in the order as they listed in window.

document.querySelector(".sessions").addEventListener("click",  handleSessionsEvent);
function handleSessionsEvent(_event){
	console.log(JSON.parse(JSON.stringify(sessionsStruct)))

	// console.log(_event.target.id);
	// alertResponse("hide");
	// document.getElementById("response").innerHTML = "";
	// document.getElementById("response").display = "none";

	if(_event.target.id == "sessions_new"){
		//alert("sessions_new");
		sessionNew();
	}	
	
	if(_event.target.id == "sessions_delete_selected"){
		//alert("sessions_delete_selected");
		sessionsStruct.sessions.splice(options.currentSession.index, 1);
		sessionsStruct.numberOfSessions--;
		
		updateIndexCS("delete");
		listManipulatorSessions(true);
		listManipulatorTabs("update", options.currentSession.index, 0);
	}

	if(_event.target.id == "sessions_rename"){	
		document.getElementById("session_name_field").value = sessionsStruct.sessions[options.currentSession.index].title; 
		document.getElementById("sessions_name").style.display = "inline";
	}

	if(_event.target.id == "sessions_rename_ok"){
		sessionsStruct.sessions[options.currentSession.index].title = document.getElementById("session_name_field").value; 
		listManipulatorSessions(true);
		document.getElementById("sessions_name").style.display = "none";
	}

	if(_event.target.id == "sessions_manage"){
		
		var tabList = document.querySelector(".tabs_manage");
		if(tabList.style.display == "none")
			tabList.style.display = "block";
		else
			tabList.style.display = "none";		
			
		tabList = document.querySelector(".tabs_list");
		if(tabList.style.display == "none")
			tabList.style.display = "block";
		else
			tabList.style.display = "none";

		
		
		// var tabList = document.querySelector(".tabs_manage").style;
		// if(tabList.display == "none")
		// 	tabList.display = "block";
		// else
		// 	tabList.display = "none";
		
	}
	/*
	if(_event.target.id.includes("session_item_")){
		alert("session_item_");
		options.currentSession = parseInt(event.target.id.match(/\d+/), 10);
		document.getElementById("debug_menu").innerHTML = options.currentSession + " " + sessionsStruct.sessions[options.currSession].sID + " " + sessionsStruct.sessions[options.currSession].name;
	}
	*/
	if(_event.target.id == "param_show_tabs_menu"){
		options.showTabsMenu = _event.target.checked; 
		if(options.showTabsMenu){
			document.querySelector(".tabs_manage").style.display = "block";
			document.querySelector(".tabs_list").style.display = "block";
		}else{
			document.querySelector(".tabs_manage").style.display = "none";
			document.querySelector(".tabs_list").style.display = "none";
		}
		toggleWideMode();
		// divResize();
	}
		
	if(_event.target.id == "param_empty_new_session"){
		options.emptyNewSession = _event.target.checked;
	}
	
	// if(_event.target.id == "param_date_in_name"){
	// 	options.chkAddDateToName = _event.target.checked;
	// }

	
	// Waiting for Workspace API from devs...
	// if(_event.target.id == "param_only_this_workspace"){
	// 	options.onlyThisWorkspace = _event.target.checked; 
	// 	alert("wp");
	// }

	if(_event.target.id == "options_clear"){ //DEV
		//alert("clear_options");
		//options.firstLaunch = true;	
		chrome.storage.local.remove('options', function(){
			alert("rem o");
		});
		
		options = Object.assign({}, optionsDefault);
	}
		
	if(_event.target.id == "sessions_clear") //DEV
	{
		// alert("sessions_clear");
		// chrome.storage.local.remove('sessionsStruct', function(){alert("ses clr");});

		chrome.storage.local.remove('sessionsStruct', function(){
			//console.log("rem"); 
			//console.log(data); 
			var error = chrome.runtime.lastError;
			if (error) {
				console.error(error);
			}
			
			// alert("ses clr");
		});
				
		sessionsStruct.numberOfSessions = 0;	
		sessionsStruct.sessions.length = 0;	
		listManipulatorSessions(true);
		// saveOptions();
	}	

	if(_event.target.id == "param_wide_mode"){ 
		options.chkWideMode = _event.target.checked;
		toggleWideMode();
	}

	if(_event.target.id == "detect"){ 
		// alert(document.getElementById("session_select").value);
		
	}


	if(_event.target.id == "response_close"){
		alertResponse("hide");
		if(options.importInProcess == true)
			cleanupImportDialog();
	}	
	
	if(_event.target.id.includes("response_dialog_")){
		dialogAnalysis(_event.target, parseInt(_event.target.id.match(/\d+/), 10)); //может лучше по содержимому?

	}
	
	saveOptions();
	_event.stopPropagation();
}


document.getElementById("session_select").addEventListener('change', function() {
	
	options.currentSession.sID = parseInt(this.options[this.selectedIndex].id.match(/\d+/), 10);
	updateIndexCS();
	listManipulatorTabs("update", options.currentSession.index, 0);
	
	
	//document.getElementById("debug_menu").innerHTML ="<br>"+ options.currentSession.sID + " " + options.currentSession.index + " " + sessionsStruct.sessions[options.currentSession.index].sID + " " + sessionsStruct.sessions[options.currentSession.index].title;
});

document.querySelector(".tabs_manage").addEventListener("click",  handleTabsEvent);
function handleTabsEvent(e){
	// alertResponse("hide");
	// document.getElementById("response").innerHTML = "";
	// document.getElementById("response").display = "none";

	if(e.target.id == "add_tabs"){
		addSeveralTabs();
	}

	if(e.target.id == "add_curr_tab"){
		addThisTab();
	}
	
	if(e.target.id == "get_all_tabs")
		getAllTabs(false);

	if(e.target.id == "delete_all_tabs"){
		updateIndexCS();
		sessionsStruct.sessions[options.currentSession.index].tabs.splice(0, sessionsStruct.sessions[options.currentSession.index].tabs.length);
		listManipulatorSessions(true);	
		listManipulatorTabs("update", options.currentSession.index, 0);
	}

	if(e.target.id == "param_add_left"){
		options.chkAddFromLeft = e.target.checked;
	}		
	if(e.target.id == "param_add_right"){
		options.chkAddFromRight = e.target.checked;
	}			
	if(e.target.id == "param_add_this_window"){
		options.chkAddOnlyThisWindow = e.target.checked;
	}		
	
	if(e.target.id == "param_title_instead_of_link"){
		options.chkTitleInsteadOfLink = e.target.checked;
		listManipulatorTabs("update", options.currentSession.index, 0);
	}	
	
	if(e.target.id == "param_URL_list_witn_numbers"){
		options.chkWithCountingNumbers = e.target.checked;
		listManipulatorTabs("update", options.currentSession.index, 0);
	}




	saveOptions();
	e.stopPropagation();
}

// document.getElementById("get_all_tabs").addEventListener("click", function(){getAllTabs(false)});

document.getElementById("list_separated").addEventListener("click",  handleListEvent);
function handleListEvent(_event){

	if(_event.target.id.includes("tab_remove_"))
		tabRemove(parseInt(_event.target.id.match(/\d+/), 10));
		
	saveOptions();
	_event.stopPropagation();
}

document.querySelector(".underground").addEventListener("click",  handleUnderground);
function handleUnderground(e){

	if(e.target.id == "export_sel"){
		showField("export");
		/*
		// alert("e");
		document.getElementById("import").style.display = "none";
		if(document.getElementById("export").style.display == "none"){
			document.getElementById("export").style.display = "block";
					
			if(getCheckedRadioElement("export_format") == "text_list")
				document.getElementById("exp_text_options").style.display = "block";
			//-------------
			if(document.getElementById("list_output").innerHTML != "Waiting..."){
				document.getElementById("list_output").style.display = "block";
				document.getElementById("list_input").style.display = "none";
				document.getElementById("list_separated").style.display = "none";
			}
		}
		else {
			document.getElementById("export").style.display = "none";
			document.getElementById("list_output").style.display = "none";
			document.getElementById("list_input").style.display = "none";
			document.getElementById("copy_to_clipboard").style.display = "none";

			//-------------
			document.getElementById("list_separated").style.display = "block";
		}
		*/
	}	

	if(e.target.id == "import_sel"){
		showField("import");
		document.getElementById("list_input").focus();
		/*
		// alert("i");
		document.getElementById("export").style.display = "none";
		document.getElementById("copy_to_clipboard").style.display = "none";

		if(document.getElementById("import").style.display == "none")
			document.getElementById("import").style.display = "block";
		else
			document.getElementById("import").style.display = "none";
		*/
	}

	if(e.target.id == "exp_on_screen")
	{	
		exportSession(getCheckedRadioElement("export_format"), getCheckedRadioElement("export_amount"));
		document.getElementById("copy_to_clipboard").style.display = "block";
		document.getElementById("list_output").focus();
	}

	if(e.target.name == "export_format"){
		if(getCheckedRadioElement("export_format") == "text_list")
			document.getElementById("exp_text_options").style.display = "block";
		else
			document.getElementById("exp_text_options").style.display = "none";	
	}

	if(e.target.id == "exp_with_indexes"){
		options.expWithIndexes = e.target.checked;
	}

	if(e.target.id == "exp_with_session_names"){
		options.expWithSessionNames = e.target.checked;
	}	
	
	if(e.target.id == "exp_decode_percent_encoding"){
		options.expDecodePercentEncoding = e.target.checked;
	}	
	
	if(e.target.id == "copy_to_clipboard"){
		window.navigator.clipboard.writeText(document.getElementById("list_output").innerHTML);
		// document.getElementById("response").innerHTML += '<font color="green">Exported session copied to clipboard!</font>';
		alertResponse("green", "Exported session copied to clipboard!");
		divResize();
	}	
	
	if(e.target.id == "im_from_input"){
		importSession(document.getElementById("list_input").value);
		// document.getElementById("response").innerHTML += '<font color="green">Exported session copied to clipboard!</font>';
		// divResize();
	}

	if(e.target.id == "im_to_this_session"){
		options.imToThisSession = e.target.checked;
	}	

	if(e.target.id == "im_string_is_ses_name"){
		options.imStringsAreNames = e.target.checked;
	}	
	
	if(e.target.id == "im_add_https"){
		options.imAddHttps = e.target.checked;
	}

	if(e.target.id == "im_sort_by_text_input"){
		options.imSortByTextInput = e.target.checked;
	}


	saveOptions();
	divResize();
}


//---LISTEN OPTIONS CHECKS---

//---EXPERIMENTS-------------------------------------

//---CODE------------------

restoreOptions(init);


function init(){
	console.log("init");
	partTimeVisibleElements.groups.push(new ptveGroupElement("default", [
		document.getElementById("list_separated")
	]));
	partTimeVisibleElements.groups.push(new ptveGroupElement("import", [
		document.getElementById("import"),
		document.getElementById("list_input")
	]));
	partTimeVisibleElements.groups.push(new ptveGroupElement("export", [
		document.getElementById("export"),
		document.getElementById("list_output"),
		document.getElementById("exp_text_options"),
		document.getElementById("copy_to_clipboard")
	]));
	console.log("ahaha init");
	// cleanupImportDialog();
	// console.log(options.importInProcess);
	if (options.importInProcess == true){
		cleanupImportDialog();
	}
}



function updateIndexCS(mode){ //CS -- Current Session
//just updates index of session to shown as selected
	
	var lim = sessionsStruct.sessions.length;
	//alert(lim);

	if(!lim){
		//alert("!lim");
		options.currentSession.index = 0;	
		options.currentSession.sID = 0;

		saveOptions();
		return;
	}

	for(var i = 0; i < lim; i++){ 
		// console.log(options.currentSession.sID);
		// console.log(options.currentSession.index);
		// console.log(sessionsStruct.sessions[i].sID);
		
		
		if (options.currentSession.sID == sessionsStruct.sessions[i].sID){
			options.currentSession.index = i;
			// console.log("ee");

			saveOptions();
			return;
		}
	}

	//ЭТУ ЧАСТЬ ПОТОМ ДОДЕЛАТЬ КОГДА СДЕЛАЮ УДАЛЕНИЕ СЕССИИ
	//Сделай перемещение на номер сессии рядом с той, с которой удалял!
	// alert(options.currentSession.sID);
	options.currentSession.index = lim-1;
	options.currentSession.sID = sessionsStruct.sessions[lim-1].sID;

	if(mode != "delete"){
		alert("Selected last time session has been removed! First selected instead.");
		options.currentSession.index = 0;
		options.currentSession.sID = sessionsStruct.sessions[0].sID;
	}

	saveOptions();

	console.log(options.currentSession.sID);
	console.log(options.currentSession.index);

	
}

function withZero(input){
	if (input < 10) 
		input = "0" + input;
	return input;
}

function getCheckedRadioElement(name)
{
	var elements = document.getElementsByName(name);
	for(i = 0; i < elements.length; i++){
		if (elements[i].checked)
			return elements[i].value;
	}
}

function dialogAnalysis(e, answerOption){
	//может лучше answerOption смотреть по содержимому (тексту кнопку)?

	// function addTemp(){
	// погоди пока с этой функцией
	// }

	// console.log(e);
	// console.log(e.parentNode);
	// console.log(e.classList);
	// console.log(e.parentNode.classList);
	
	if((e.classList.contains("response-btn") && e.closest(".response").classList.contains("importFailedJSON"))){
		disableButtons("dialogImportJSON", true);
		console.log("check me");



		// console.log(JSON.parse(JSON.stringify(sessionsStruct)))

		if(answerOption == 0){//yes
			//ПОЧТИ РАБОТАЕТ, НЕ ЗАБУДЬ ПРИКРУТИТЬ К КОРРЕКТНОМУ ДОБАВЛЕНИЮ JSON И СПИСКА 

			//function not ready
			// addSessionsToList(messageHolded.preStruct);
			// console.log("YES");
			

			//если сессия выведена на экран, вохможно там отредактирована, то добавлять её. Если нет, то начисто создаём новую


			// if(	!(sessionsStruct.sessions[sessionsStruct.sessions.length - 1].sID == messageHolded.preStruct.sessions[0].sID)
			// 	&& 	!(options.importInProcess == true))
			// {
			// 	addTemp();
			// }
			// for(var i = sessionsStruct.sessions.length-1; i >= 0; i--){
			// 	if((sessionsStruct.sessions[i].sID == options.currentSession.sID) && (sessionsStruct.sessions[i].isTemp == true)){
			// 		sessionsStruct.sessions[i].isTemp == false
			// 		break;
			// 	}
			// }

			if(options.importInProcess == true){
				sessionsStruct.sessions[options.currentSession.index].isTemp = false;
				showField("import");
			}
			else
				addSessionsToList(messageHolded.preStruct); 

			messageHolded = false;
			cleanupImportDialog();
			// setTimeout( cleanupImportDialog, 1000);
			alertResponse("hide");
			showField("import");
			// disableButtons("dialogImportJSON", false);
		}		
		
		if(answerOption == 1){//no
			if(options.importInProcess == true){
				showField("import");
			}
			messageHolded = false;
			cleanupImportDialog();
			alertResponse("hide");
			// showField("import");
			// disableButtons("dialogImportJSON", false);
		}		
		
		if(answerOption == 2){//show
			//ВАС УСТРАИВАЕТ? ДА НЕТ ИЛИ ПРОСТО УБРАТЬ ОПЦИЮ
			options.importInProcess = true;
			saveOptions();
			// console.log(options.importInProcess);
 			// console.log("LEMME SEE");
 			// console.log(options.importInProcess);
			//добавить сессию как новую, не обновляя список сессии, показать её, удалить. 
			
			// addTemp();
			//костыль
			messageHolded.preStruct.sessions[0].isTemp = true; //to prevent asyncronious issues with saveOptions
			sessionsStruct.sessions.push(messageHolded.preStruct.sessions[0]);
			options.currentSession.sID = sessionsStruct.sessions[sessionsStruct.sessions.length - 1].sID;
			updateIndexCS();

			
			listManipulatorTabs("update",  options.currentSession.index, 0);
			showField("default");
			removeAlertDialogButton(answerOption);
			//СДЕЛАЙ ТАК ЧТОБЫ АЛЁРТ ЗАКРЫВАЛСЯ
			
			//НЕ ЗАБУДЬ УДАЛИТЬ СЕССИЮ, СДЕЛАТЬ ДОБАВЛЕНИЕ СЕССИИ (ПЕРЕСОХРАНЯЙ MESSAGEHOLDED)

			//--- УБЕРИ ЗАСЕРИВАНИЕ ПОСЛЕ ЗАКРЫТИЯ АЛЁРТА КРЕСТИКОМ (РАВНОСИЛЬНО NO) -> //не добавлять крестик при предпоказе (или сделать фичей?).
			//если фича, то надо сессию не удалять, а опять в messageHolded и из него записывать в память //лучше запоминать содержимое поля

		}
	}
}

function cleanupImportDialog(){//НЕ РАБОТАЕТ ПРАВИЛЬНО //а как должно
	
	for (var index = 0; index < sessionsStruct.sessions.length; index++){
		console.log(index);
		console.log(JSON.parse(JSON.stringify(sessionsStruct)));
		if (sessionsStruct.sessions[index].isTemp == true){
			console.log("clean up " + index + " " + sessionsStruct.sessions[index].sID); 
			sessionsStruct.sessions.splice(index, 1);
			index--;
		}		
	}
	
	updateIndexCS("delete");
	listManipulatorSessions(true);
	listManipulatorTabs("update",  options.currentSession.index, 0);
	disableButtons("all", false);
	options.importInProcess = false;
	saveOptions();
}

function removeAlertDialogButton(answerOption){
	document.getElementById("response_dialog_" + answerOption).remove();
}

function alertResponse(mode, message, answerMode, buttons, buttonsToDisable, hideCross){
	/*
	buttons = [name1, color1, name2, color2, ...]
	*/
	answerModes = ["importFailedJSON"];

	var htmlButtons = "";

	e = document.getElementById("response");

	e.innerHTML = "";

	if(answerMode){
		e.classList.add(answerMode);
	}
	if (mode == "hide"){
		e.style.display = "none";
		
		e.classList.remove("green");
		e.classList.remove("red");
		answerModes.forEach(function(modeName){e.classList.remove(modeName);});
		return;
	}
	if(mode == "green"){
		e.classList.remove("red");
		e.classList.add("green");
	}
	if(mode == "red"){
		e.classList.remove("green");
		e.classList.add("red");
	}


	if(buttons){
		// alerted = true;
		// console.log("buttons");
		// htmlButtons = '<div style="display: table;">';
		htmlButtons = '<div style="width: 100%; text-align: center;">';
		for(var i = 0, lim = buttons.length - 1; i < lim; i += 2){
			htmlButtons += '<button class="response-btn" style=" background : ' + buttons[i+1] + '; " id="response_dialog_' + i/2 +'">' + buttons[i] + '</button>';
		}
		htmlButtons += '</div>';
		
		disableButtons(buttonsToDisable, true);

	}
	// e.innerHTML += message + '\n<div class="response-closebtn" id="response_close">&times;</div>';
	
	if(hideCross)
		cross = '';
	else
		cross = '<div class="response-closebtn" id="response_close">&times;</div>';

	e.innerHTML += cross + message + htmlButtons;
	e.style.display = "block";
}
// alertResponse("red", "bruh");

function sessionNew(){
	
	//alert ("a");
	
	var sID = new Date().getTime();	
	var now = new Date();
	var dateFormatted = withZero(now.getDate()) + "." + withZero(now.getMonth() + 1) + "." + now.getFullYear() + " " + withZero(now.getHours()) + ":" + withZero(now.getMinutes());

	// console.log(now.getMonth());

	var title = "";
	
	if(options.chkAddDateToName)
		title += dateFormatted;	
	
	//это доделай!!
	// Waiting for Workspace API from devs...
	// if(options.onlyThisWorkspace){

	// }

	document.getElementById("session_name_field").value = title;

	sessionsStruct.sessions.push(new Session(sID, title, 0, now, []));
	sessionsStruct.numberOfSessions++;
	
	options.currentSession.sID = sID;
	updateIndexCS();

	// document.getElementById("response").innerHTML = '<font color="green">New session created!</font>';
	alertResponse("green", "New session created!");
	// document.getElementById("response").display = "inline";
	divResize();

	if (!options.emptyNewSession)
		getAllTabs(false);
	else{
		listManipulatorSessions(true);
		listManipulatorTabs("update", options.currentSession.index, 0);
	}
	// listManipulatorTabs("update", options.currentSession.index, 0);
	// listManipulatorSessions(true);

	// console.log(sessionsStruct);
	
}


function listManipulatorTabs(mode, sIndex, startFrom){

	//ПОМЕНЯЙ СТРУКТУРУ
	
	//alert("lM");

	if(!sessionsStruct.sessions.length){
		document.getElementById("list_separated").innerHTML = '<div class="list_elem" style="border : 0px; color : red;">No tabs! You should add a session at first! </div>';
		return;
	}
	
	var preList = "";
	var i = startFrom;
	var lim = sessionsStruct.sessions[sIndex].tabs.length;
	var numberField = "";


	
	if(!sessionsStruct.sessions[sIndex].tabs.length)
		preList = '<div class="list_elem" style="border : 0px;"> No tabs! </div>';
	
	if(mode == "getAllTabs" || "tabRemove" || "update")
	{
		
		for (i = 0; i < lim; i++){
			if(options.chkWithCountingNumbers) //передалать в отдельный элемент и скрывать его?
				numberField = sessionsStruct.sessions[sIndex].tabs[i].index + 1 + ' ' ; 
			//link or title
			if (options.chkTitleInsteadOfLink){ 
				itemContent = sessionsStruct.sessions[sIndex].tabs[i].title;
				titleContent = sessionsStruct.sessions[sIndex].tabs[i].URL;
			}			
			else{ 
				itemContent = sessionsStruct.sessions[sIndex].tabs[i].URL;
				titleContent = sessionsStruct.sessions[sIndex].tabs[i].title;
			}
			
			preList +=
				'<div class="list_elem" id=item_'+ i +'>'
				// + '<input type="button" id="tab_remove_'+ i +'" value="X" style="float: left; margin-right: 2px;" title="Delete tab">
				// + '<div class="inlinebtn" id="tab_remove_'+ i +'">X</div>'
				// + '<div class="inlinebtn" id="tab_remove_'+ i +'">&times;</div>'
				+ '<span class="inlinebtn" id="tab_remove_'+ i +'">&times;</span>'
				//СДЕЛАЙ DRAG AND DROP!!!!!!
				// + '<input type="button" id="tab_up_'+ i +'" value="↑" style="float: left;" title="Move tab up">'
				// + '<input type="button" id="tab_down_'+ i +'" value="↓" style="float: left;" title="Move tab down">'
				+ numberField 
				+ '<a href="' + sessionsStruct.sessions[sIndex].tabs[i].URL + '" title="' + titleContent + '">' + itemContent + '</a>'
				+ '</div>\n';
			
		}
	}

	var listDOMel = document.getElementById("list_separated");
	listDOMel.style.height = null; 
	listDOMel.innerHTML = preList;
	// if(document.getElementById('list_output').rows < 25)
	// 		document.getElementById('list_output').rows++;
	divResize();
	saveOptions();
		//restoreOptions();

}

function listManipulatorSessions(mode){
	
	var i = 0;

	var preList = "";
	var isSelected = "";
	var lim = sessionsStruct.sessions.length;
	//var numberField = "";
	//var chkTitleInsteadOfLink = false;
	
	updateIndexCS();

	if(!lim){
		preList = '<option>No sessions!</option>\n';
		mode = null;
	}
	
	if(mode)
	{
		// var nameField = document.getElementById("sessions_name");
		// nameField.style.display = "inline";

		//скорее всего потребуется переход на for each или смещение номеров сессий
		for (i = 0; i < lim; i++){
			if (sessionsStruct.sessions[i].sID == options.currentSession.sID)
				isSelected = 'selected';

			preList +=
				// '<option class="list_elem" id="session_item_'+ i +'" ' + isSelected + '>'
				// + '<input type="button" id="session_remove_'+ i +'" value="X" style="float: left;" title="Delete session">'
				'<option id="session_item_'+ sessionsStruct.sessions[i].sID +'" ' + isSelected + '>'
				+ sessionsStruct.sessions[i].title + ', ' + sessionsStruct.sessions[i].tabs.length + ' tabs'
				+ '</option>\n';
			
			isSelected = "";
		}
		
	}
	document.getElementById("session_select").innerHTML = preList;
	saveOptions();
}