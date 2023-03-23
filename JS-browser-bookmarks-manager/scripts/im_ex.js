function text_out(output){
	if(!output){
		alertResponse("red", "Nothing to export.");
		return;
	}
	document.getElementById("list_separated").style.display = "none";
	document.getElementById("list_output").innerHTML = output;
	document.getElementById("list_output").style.display = "block";
	document.getElementById("list_output").focus();
}

function convertToText(s){
	// alert(s);
	console.log(s);
	var text_list = "";

	for(i = 0, lim = s.sessions.length; i < lim; i++){
		//сделать отсечение дополнительной инфы
		if(options.expWithSessionNames) 
			text_list += s.sessions[i].title + " , " + sessionsStruct.sessions[i].tabs.length + ' tabs ' + s.sessions[i].date + " " + s.sessions[i].sID + "\n";
		
		// if(options.expWithSessionsExtraInfo)
			// text_list += " , " + sessionsStruct.sessions[i].tabs.length + ' tabs ' + s.sessions[i].date + " " + s.sessions[i].sID + "\n";

		for(j = 0, jlim = s.sessions[i].tabs.length; j < jlim; j++){
			if(options.expWithIndexes)
				text_list += s.sessions[i].tabs[j].index + " ";
				
			if (options.expDecodePercentEncoding)
				text_list += decodeURI(s.sessions[i].tabs[j].URL)  + "\n";
			else
				text_list += s.sessions[i].tabs[j].URL + "\n";
			
		}
		if(text_list)
			text_list += "\n";

	}
	
	// console.log(text_list);
	return text_list;

}

function convertToJSON(s){

	return JSON.stringify(s, null, " ");
	// return JSON.stringify(s);
}



function exportSession(format, amount, indexes){
	var s_i = []; //indexes of imported sessions

	if(amount == "exp_only_this")
		s_i.push(options.currentSession.index);
	if(amount == "exp_all")
		for(i = 0, lim = sessionsStruct.sessions.length; i < lim; i++){
			s_i.push(i);
		}
	if(amount == "exp_several")
		s_i = indexes;

	// console.log(s_i);
	// console.log(with_indexes);
	//обработать s_i
	var exp_sessions = {
		numberOfSessions : 0,
		sessions : []
	};
	
	for (i = 0; i < s_i.length; i++){
		exp_sessions.sessions.push(sessionsStruct.sessions[s_i[i]]);
	}

//--------
	if(format == "text_list"){
		// sessionsStruct.sessions[
			text_out(convertToText(exp_sessions));

			/*for dev purposes, should be commented*/
			console.log(document.querySelectorAll('select, input[id]'));
			var id_list = '[';
			document.querySelectorAll('select, input[id], textarea').forEach(function(el){
				id_list += '"' + el.id + '",\n' 
			});
			id_list += ']';
			text_out(id_list);
	}	
	if(format == "JSON"){
		// sessionsStruct.sessions[
			text_out(convertToJSON(exp_sessions));
	}


}


function tryTextImport(input){
	console.log("text", input);

	var strings = input.match(/(.*[^\n])/g);

	console.log("parsed", JSON.parse(JSON.stringify(strings)));



	//returning object
	msg = {
		status : "",
		content : "", 
		preStruct : ""
	}

	//clearing from empty strings (spaces, tabs)
	// if(options.clearEmptyLines)
		for(var i = 0, lim = strings.length; i < lim; i++){
			// console.log(strings[i], " ", i);
			if(strings[i].search(/.*[^\s]/)){ //idk how it exactly works, should be >0 i guess
				// console.log(strings[i], " ", i);
				// console.log("del");
				strings.splice(i, 1);
				lim--;
				i--;
			}
		}
	// console.log("text", strings);
	
	//clearing from spaces
	//перенести в цикл сверху
	for(var i = 0, lim = strings.length; i < lim; i++){
		strings[i] = strings[i].trim();
	}
	console.log("trimmed", JSON.parse(JSON.stringify(strings)));

	//выделить название сессии??
	/* если в строке есть есть число из 13 цифр, отделённое пробелами, то это -- sID, оно же время
	* 16.11.2020 21:27 2020-12-16T18:27:07.693Z 1608143227693
	*                                          ^             ^
	* названия сессий могут начинаться с {[
	*/

	//но как запихнуть названия?


	//JSON similarity check
/*
	console.log("similarity");
	reg = /".*":.*[\,\[]/;
	res = input.match(reg);
	for(const ith of res){
		console.log("\n AAAA = ", ith);
	}
*/

	//marks for JSON moments, storing the line where occurs
	var brackets = -1;
	var field = -1;
	// search for fields:  (?:".*?":[^,\[\{\]\}]*)
	// search for brackets on line: ^\s*[\[\{\]\}]+(?:\n|$)
	//summary :
	reField = /".*?":[^,\[\{\]\}]*/;
	reBrackets = /^\s*[\[\{\]\}]+(?:\n|$)/;

	for(lim = strings.length, i = 0; i < lim; i++){

		console.log(reField.test(strings[i]));
		console.log(reBrackets.test(strings[i]));

		if(field == -1 && reField.test(strings[i]))
			field = i;
		if(brackets == -1 && reBrackets.test(strings[i]))
			brackets = i;

		console.log(field, brackets);
		if(field != -1 && brackets != -1)
			break;
	}

	if(field + brackets != -2){
		msg.status = "error";
		msg.content = [brackets, field];
	}



	//make structure for future merging
	var preStruct = {
		numberOfSessions : 0,
		sessions : [] //last session = last saved
		
	};
	
	//creating new session for imported text
	//but what if session already exist?
	var result;
	var currentSession = -1;
	var title = "";
	var i = 0;

	var sID = new Date().getTime();	
	var now = new Date();
	var dateFormatted = withZero(now.getDate()) + "." + withZero(now.getMonth() + 1) + "." + now.getFullYear() + " " + withZero(now.getHours()) + ":" + withZero(now.getMinutes());

	//if first line is the name of session
	if(options.imStringsAreNames){
		title = strings.shift();
	}
	title += " " + dateFormatted;	

	preStruct.sessions.push(new Session(sID, title, 0, now, []));
	preStruct.numberOfSessions++;
	currentSession++; // For future //для какого ещё будщего


	// console.log(strings.length, " i=", i)
	//split text indexes from link if exists and write them to localBrowserID
	for(lim = strings.length; i < lim; i++){


		var index = preStruct.sessions[currentSession].tabs.length;
		var lindex = -1;
		result = strings[i].match(/^\d+\s/);
		
		if(result){
			lindex = parseInt(result[0], 10);
			strings[i] = strings[i].replace(result[0], "");
		}

		if(options.imAddHttps){
			// console.log("checking...") ;
			// if(! (/^https:\/\//.test(strings[i]) || /^https:\/\//.test(strings[i]))){
			if(! /^https:\/\/|^http:\/\//.test(strings[i]) ){
				strings[i] = "http://" + strings[i];	
				// console.log("add");
			}
		}

		preStruct.sessions[currentSession].tabs.push(new Tab(
			index, // preStruct.sessions[currentSession].tabs.length, 
			lindex, //preStruct.sessions[currentSession].tabs.length,
			strings[i], strings[i]));

		//document.getElementById("import").innerHTML += "  " + i +  " " + preStruct.sessions[0].tabs[i].URL + "\n";
	}


	/*
	if(false) //мне лень комментить --  но это должен быть парс JSON
		for(var i = 0, lim = strings.length; i < lim; i++){
			
			if(strings[i].search(/.*\".*\".*:.*\".*\"/))
				console.log("f");
			if(strings[i] == ("{" || "}" || "[" || "]" 
				// ищем поля /.*\".*\".*:.*\".*\"/
				|| strings[i].search(/.*\".*\".*:.*\".*\"/)
				) )
					console.log("json fail");

			//если нет ://, то это название
			if(strings[i].search(/:\/\//) == -1){
				console.log("name: ", strings[i]);


				//detect sID, convert to time
				var sID;
				var time = new Date();

				// var re = /\d{13,13}/;
				// result = re.exec(strings[i]);
				result = strings[i].match(/\d{13,13}/);
				// console.log(result);

				if(result){
					//  console.log("1=", strings[i][result.index - 1], " 2=", strings[i][result.index + 13]);
					//  if(strings[i][result.index - 1] == (undefined || "" || " "))
					// 	 console.log("1 pass");
					// if(strings[i][result.index + 13] == (undefined || "" || " " || "\n"))
					// 	 console.log("2 pass");
					// if(strings[i][result.index + 13] == undefined)
					// 	 console.log("2 u");

					if( ((strings[i][result.index - 1] == ("" || " ")) 
						|| (strings[i][result.index - 1] == undefined) )
						&& ( (strings[i][result.index + 13] == ("" || " " || "\n")) 
						|| (strings[i][result.index + 13] == undefined) ) )
					{				
						sID = result[0];
						time.setTime(sID);

						// console.log(sID, " ", time);
					}
				}
				// sID = new Date().getTime();	
				// var time = new Date();

				//making ses

				var title = strings[i];
					
				preStruct.sessions.push(new Session(sID, title, 0, time, []));
				preStruct.numberOfSessions++;
				currentSession++;
			}else{
				if(!preStruct.sessions.length){
					console.log("no sessions in text, create new");
					var sID = new Date().getTime();	
					var now = new Date();
					var dateFormatted = withZero(now.getDate()) + "." + withZero(now.getMonth() + 1) + "." + now.getFullYear() + " " + withZero(now.getHours()) + ":" + withZero(now.getMinutes());
				
					var title = "imported " + dateFormatted;
					preStruct.sessions.push(new Session(sID, title, 0, now, []));
					preStruct.numberOfSessions++;
					currentSession = 0;
				}

				var index;

				// console.log(preStruct);
				result = strings[i].match(/^\d+\s?/);
				if(result){
					// console.log(result);
					// console.log(result[0].length);
					// strings[i] = strings[i].slice(0, result[0].length);
					strings[i] = strings[i].replace(result[0], "");
					index = parseInt(result[0], 10);
					// console.log(strings[i]);
					// console.log(index);

				//  я думаю, это перенести потом в проверку массива и менять уже там, сортировать
				// 	if(preStruct.sessions[currentSession].tabs.length)
				// 		for (var j = 0, jlim = preStruct.sessions[currentSession].tabs.length; j < jlim; j++){
				// 			if(preStruct.sessions[currentSession].tabs[j].index == index){
				// 				index = preStruct.sessions[currentSession].tabs.length;
				// 				break;
				// 			}
				// 		}
					
				}
				else{
					index = preStruct.sessions[currentSession].tabs.length;
				}
				
				
				preStruct.sessions[currentSession].tabs.push(new Tab(
										index, // preStruct.sessions[currentSession].tabs.length, 
										index, //preStruct.sessions[currentSession].tabs.length,
										strings[i], strings[i]));

			}
		}	
	}	
	// }
	*/
	

	// console.log("before sort");
	// console.log(preStruct);	
	
	/* TODO:
	 * checking if indexes not colliding, fix it
	 * checkedindexes: 2d array, where 1st dimension is checked index, and second dimension is elements with this index
	 */
	/*
	function collision ( ){
		index = 0;
		realIDs = [];

	}
	var collisions = []; 

	var checkedIndexes = [];

	// for(i = 0, lim = preStruct.tabs.length; i < lim; i++){
	// 	checkedIndexes[i] = preStruct.sessions[currentSession].tabs[i].index;

	// 	for(var j = 0; j < checkedIndexes.length; j++){
			
	// 	}
	// }
	for(i = 0, lim = preStruct.sessions[currentSession].tabs.length; i < lim; i++){
		if(!Array.isArray(checkedIndexes[preStruct.sessions[currentSession].tabs[i].index]))
			// console.log(i);
			checkedIndexes[preStruct.sessions[currentSession].tabs[i].index] = [];
		checkedIndexes[preStruct.sessions[currentSession].tabs[i].index].push(i);

	}
	console.log(checkedIndexes);
	*/

	console.log("before check:");
	console.log( JSON.parse(JSON.stringify(preStruct)));
	
	checkPreStruct(preStruct);
//temp
	if (msg.status != "error")
		msg.status = "succ";
	msg.preStruct = preStruct;

	console.log(msg);
	return msg;
}

//checking preStruct as it made by TextImport or JSON
function checkPreStruct(preStruct){

	console.log("checkprestruct:");
	console.log(JSON.parse(JSON.stringify(preStruct)));

}

function addSessionsToList (preStruct){ // принимает messageHolded.preStruct
	//i was thinking about adding multiple sessions but not now, so i just left this function
	//in fact this function has no sense
	// sessionsStruct.sessions.push(session);
	//messageHolded.preStruct.sessions[0].isTemp = false;
	
	preStruct.sessions.forEach(function(el){
		// console.log("addSessionsToList:");
		// console.log(JSON.parse(JSON.stringify(preStruct)));
		// console.log(JSON.parse(JSON.stringify(el)));
		el.isTemp = false;
		sessionsStruct.sessions.push(el);
		sessionsStruct.numberOfSessions++;
		options.currentSession.sID = el.sID;
		
		messageHolded = false;
	});
}



function importSession(input){
	//если уже сть сессия с таким sID от времени, то выдать предупреждение о том, что нужна корректировка
	var preStruct; //может сделать её глобальной?
	// console.log("in ", input);

	//preStruct = JSON.parse(input);



	//НАПИШИ ПРОВЕРКУ НАЛИЧИЯ ПОЛЕЙ [каких ещё полей] И В ОТКРЫТИЕ ВКЛАДОК ЭТО ТОЖЕ УЧТИ
	if(!input){
		alertResponse("red", "Nothing to import.");
		return;
	}
	try{
		// preStruct = JSON.parse("fdf");
		preStruct = JSON.parse(input);
		if(preStruct){
			checkPreStruct(preStruct);
			alertResponse("green", "Imported as JSON.");
		}
		console.log(preStruct);
	}
	catch(error){
		//вернёт текстовый импорт ошибку или нет
		//если ошибка, то вернёт объект типа {"error", "ошибка в строке такой-то"}
		//если успех, то {"succ", preStruct}
		msg = tryTextImport(input);
		if (msg.status == "error"){
			var alertText = '<span style="color:red; font-weight: bold;">ERROR!</span> It looks like you are trying to import JSON, but it cannot be parsed.<br>';
			if(msg.content[0] != -1)
				alertText += 'Only brackets found in line <b>' + msg.content[0] + '</b> and probably further.<br>';
			if(msg.content[1] != -1)
				alertText += 'JSON-like field and key found in line <b>' + msg.content[1] + '</b> and probably further.<br>';
			// alertText += 'Please check syntax and fields! You may forgot to copy brackets at the beginning or at the end. <b> Here\'s JSON parser error:</b><br><i>' + error + '</i>';
			alertText += 'Please check syntax and fields! You may forgot to copy brackets at the beginning or at the end. <b> Here\'s JSON parser error:</b><div style="border: 1px dashed red; text-align: center; margin-top: 0.5em; padding: 0.5em;">' + error + '</div>';

			alertText += '<div style="text-align: center; margin-top: 0.5em;"><b>Do you want to continue anyway?</b></div>';
			buttons = ["Yes", "#1cb512", "No", "#d01b1b", "Show how it will look", "#006bb7"];
			// buttons = ["пиываолдп", "#1cb512", "пиываолдп", "#6c266e","пиываолдп", "#1cb512","пиываолдп", "#1cb512",];
			messageHolded = msg;
			alertResponse("red", alertText, "importFailedJSON", buttons, "dialogImportJSON", true);
			// alert(alertText);
			//alertResponse("red", "Cannot read as JSON! Please check syntax and fields! You may forgot to copy brackets in beginning or end. <br>" + error);
			
		}
		if (msg.status == "succ"){
			alertResponse("green", "Imported as text links list.");

			//TODO САМ ИМПОРТ
		}
		
	
		// alertResponse("red", "Cannot read as JSON! Please check syntax and fields! You may forgot to copy brackets in beginning or end. <br>" + error);
		// alertResponse("green", "Imported as text links list.");
		return;
		//console.log("see ", e);

	}

	// checkPreStruct(preStruct);
}