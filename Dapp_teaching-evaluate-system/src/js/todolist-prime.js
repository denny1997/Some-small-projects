// var count1 = 0,count2 = 0;
// var list1 = new Array();
// var list2 = new Array();
// var itemlist = new Array();
// var item_ev = new Array();
// function add(){
//     var name = document.getElementById("name"),
//         age = document.getElementById("age"),
//         mark = document.getElementById("mark");

//     item_ev.push({xinyong:age.value,note:mark.value});
//     find(name.value,age.value,item_ev.length-1);
//     load2();
//     hide_add('adddiv');
// }

// function load2(){
//     var todo = document.getElementById("todolist"),
//         todoString = "";
//     for (var i=0; i<itemlist.length; i ++){
//         todoString += "<tr>"
//                 + "<td ></td>" 
//                 + "<td >" + itemlist[i].name + "</td>" 
//                 + "<td >" + itemlist[i].xinyong + "</td>" 
//                 + "<td >" + itemlist[i].num + "</td>" 
//                 + "<td ></td>" 
//                 +"<td>"
//                 +"<a href='#' class='btn btn-success'  onclick='show_evlist("+i+")'><i class='fa fa-check'></i></a>"
//                 +"</td>"
//                 +"</tr>"; 
//     }
//     todo.innerHTML = todoString;
// }
// function show_evlist(j){
//     var todo = document.getElementById("evlist"),
//         text = document.getElementById("evname"),
//         temp,
//         todoString = "";
//     text.innerHTML = "用户:" + itemlist[j].name;
//     for (var i=0; i<itemlist[j].list.length; i ++){
//         temp = itemlist[j].list[i];
//         todoString += "<tr>"
//                 + "<td >" + i+1 + "</td>" 
//                 + "<td >" + item_ev[temp].xinyong + "</td>" 
//                 + "<td >" + item_ev[temp].note + "</td>" 
//                 +"</tr>"; 
//     }
//     todo.innerHTML = todoString;
//     show_add("ev");
// }
// function find(a,b,c){
//     for (var i=0; i<itemlist.length; i ++){
//         if(itemlist[i].name == a){
//             itemlist[i].list.push(c);
//             itemlist[i].xinyong = Number(itemlist[i].xinyong)+Number(b);
//             itemlist[i].num = itemlist[i].num+1;
//             return i;
//         }
//     }
//     var empty = new Array();
//     empty.push(c);
//     itemlist.push({name:a,xinyong:b,num:1,list:empty});
// }

// function request(){
//     var name = document.getElementById("newname"),
//         department = document.getElementById("department"),
//         job = document.getElementById("job"),
//         note = document.getElementById("note"),
//         mig = document.getElementById("mig"),
//         sender = "123456789 <br/>";

//     if(name.value!="" && department.value!="--" && job.value!="--" && note.value!=""){
//         count1++;
//         list2.push({a:name.value,b:department.value,c:job.value,d:note.value});
//         load();
//         hide_add('requdiv');
//     }else {
//         alert("不能为空");
//     }
// }

// function update(i){
//     var todo = document.getElementById("rightlist"),
//         todoString = todo.innerHTML;
//     count2++;
//     list1.push({a:list2[i].a,b:list2[i].b,c:list2[i].c});
//     remove2(i);
//     todo.innerHTML = todoString;
//     load();

// }

// function load(){
//     var right = document.getElementById("rightlist"),
//         requ = document.getElementById("requlist"),
//         todoString1 = "",
//         todoString2 = "";
//     for (var i=0; i<list1.length; i ++){
//         todoString1 += "<tr>"
//                 + "<td id='p-"+i+"'></td>" 
//                 + "<td id='p-"+i+"'>" + list1[i].a + "</td>" 
//                 + "<td id='p-"+i+"'>" + list1[i].b + "</td>" 
//                 + "<td id='p-"+i+"'>" + list1[i].c + "</td>" 
//                 +"<td>"
//                 +"<a href='#' class='btn btn-danger' onclick='remove1("+i+")'><i class='fa fa-times'></i></a>"
//                 +"</td>"
//                 +"</tr>";  

//     }
//     for (var i=0; i<list2.length; i ++){
//         var department=list2[i].b,
//         job = list2[i].c;
//         todoString2 += "<tr>"
//                 + "<td id='p-"+i+"'></td>" 
//                 + "<td id='p-"+i+"'>" + list2[i].a + "</td>" 
//                 + "<td id='p-"+i+"'>" + list2[i].b + "</td>" 
//                 + "<td id='p-"+i+"'>" + list2[i].c + "</td>" 
//                 + "<td id='p-"+i+"'>" + list2[i].d + "</td>" 
//                 +"<td>"
//                 +"<a href='#' class='btn btn-success' onclick='update("+i+")'><i class='fa fa-check'></i></a>"
//                 +"<a href='#' class='btn btn-danger' onclick='remove2("+i+")'><i class='fa fa-times'></i></a>"
//                 +"</td>"
//                 +"</tr>";  
//     }
//     right.innerHTML = todoString1;
//     requ.innerHTML = todoString2;
// }
// function remove1(i){
//     list1.splice(i,1);
//     load();
// }
// function remove2(i){
//     list2.splice(i,1);
//     load();
// }
// function show(obj,tex){
//     var box = typeof obj == "string" ? document.getElementById(obj) : obj,
//         text= typeof tex == "string" ? document.getElementById(tex) : tex;
//     if(box.style.display=="none"){
//         box.style.display = "block";
//         //text.innerHTML = "-";
//     }else {
//         box.style.display = "none";
//         //text.innerHTML = "+";
//     }
    
// }
// function show_add(obj){ 
//     var popUp = typeof obj == "string" ? document.getElementById(obj) : obj;
//     popUp.style.visibility = "visible"; 
// } 
// function hide_add(obj){ 
//     var popUp = typeof obj == "string" ? document.getElementById(obj) : obj;
//     popUp.style.visibility = "hidden"; 
// } 