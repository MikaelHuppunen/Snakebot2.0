let c = document.getElementById("s").getContext("2d"),j=32, X = 288,Y = 320, 
score = 0,H = 0,s,b,d, att = 0,crossoverpoint, timer = 0, totalfit, foodx = []
, foody = [], dmin = 15, n = [{x:X,y:Y}], gensize = 200,
tries = 50, dstart, parentrank = [], deathkey = 0;
let value = [];
let multiplier = [];
const fitness = [];
let parentnum = [];
let parentmultiplier = [];
let bias = [];
let parentbias = [];

//input weights and biases
document.getElementById("fileInput1").addEventListener("change", function (event) {
    const file = event.target.files[0];

    if (file) {
        const reader = new FileReader();

        reader.onload = function (e) {
            const text = e.target.result;
            bias[0] = JSON.parse(text);
        };

        reader.readAsText(file);
    }
});

document.getElementById("fileInput2").addEventListener("change", function (event) {
    const file = event.target.files[0];

    if (file) {
        const reader = new FileReader();

        reader.onload = function (e) {
            const text = e.target.result;
            multiplier[0] = JSON.parse(text);
        };

        reader.readAsText(file);
    }
});

//training time 45h47
for(let i = 0; i < 5; i++){
	value[i] = [];
}


function food(){
	for(let i = 0; i < 260; i++){
		foodx[i] = Math.floor(Math.random()*17+1) * j;
		foody[i] = Math.floor(Math.random()*15+3) * j;
	}
}
food();

dstart = (Math.abs(X-foodx[0]) + Math.abs(Y-foody[0]))/32;
dmin = dstart;
while(Collision(foodx[0],foody[0])){
	foodx[0] = Math.floor(Math.random()*17+1)*j;
	foody[0] = Math.floor(Math.random()*15+3)*j;
}

let output;

for(let i = 0; i < 510; i++){
	value[0][i] = 0;
}

function ai(){
	for(let i = 0; i < score; i++){
		value[0][i]=(n[i+1].x-32)/544;
		value[0][255+i] = (n[i+1].y-96)/480;
	}
	value[0][510] = ((X-32)/32)/17;
	value[0][511] = ((Y-96)/32)/15;
	value[0][512] = ((foodx[score]-32)/32)/17;
	value[0][513] = ((foody[score]-96)/32)/15;
	value[0][514] = (d==1);
	value[0][515] = (d==2);
	value[0][516] = (d==3);
	value[0][517] = (d==4);
	for(let i = 0; i < 32; i++){
		value[1][i] = bias[att][0][i];
		for(let ii = 0; ii < 518; ii++){
			value[1][i] += multiplier[att][0][i][ii]*value[0][ii];
		}
		value[1][i] = Math.max(0, value[1][i]);
	}
	for(let i = 0; i < 16; i++){
		value[2][i] = bias[att][1][i];
		for(let ii = 0; ii < 32; ii++){
			value[2][i] += multiplier[att][1][i][ii]*value[1][ii];
		}
		value[2][i] = Math.max(0, value[2][i]);
	}
	for(let i = 0; i < 8; i++){
		value[3][i] = bias[att][2][i];
		for(let ii = 0; ii < 16; ii++){
			value[3][i] += multiplier[att][2][i][ii]*value[2][ii];
		}
		value[3][i] = Math.max(0, value[3][i]);
	}
	for(let i = 0; i < 4; i++){
		value[4][i] = bias[att][3][i];
		for(let ii = 0; ii < 8; ii++){
			value[4][i] += multiplier[att][3][i][ii]*value[3][ii];
		}
		value[4][i] = Math.max(0, value[4][i]);
	}
	output = value[4].indexOf(Math.max(...value[4]));
	if((output + 1)%2 != d%2 || d == 0){
		d = output + 1;
	}
}

let pressed = 0;

document.addEventListener('keydown', function(e) {
	if(pressed == 1){
		return;
	}
	pressed = 1;
	let k = e.keyCode;
	if(k == 13){
		if(g == undefined){
			var g = setInterval(D, 40);
		}else{
			clearInterval(g);
			g = undefined;
		}
	}
	if(k == 39){
		deathkey = 1;
	}
})

document.addEventListener('keyup', function(e) {
	pressed = 0;
})

function Collision(a,b){
    for(let i=0; i < n.length; i++){
        if(a==n[i].x&&b == n[i].y){
            return 1
        }
    }
}

function draw_circle(x, y){
	c.beginPath();
	c.arc(x + 16, y + 16, 16, 0, 7);
	c.fill()
}

function set_fillstyle(fillstyle){
	c.fillStyle = fillstyle;
}


function D(){
	timer++;
	ai();
	set_fillstyle("#4a752c");
	c.fillRect(0, 0, 608, 72);
	set_fillstyle("#578a34");
	c.fillRect(0, 72, 608, 536);
	set_fillstyle("#aad751");
	c.fillRect(j, 96, 544, 480);
	c.rect(j, 0, 576, 576);
	c.clip();
	for(let i = 999; i > 0; i--){
		set_fillstyle("#a2d149");
		c.fillRect((2*i-Math.floor(i/9)*18+Math.floor(i/9)%2)*j, Math.floor(i/9)*j+96, j, j)
	}
	set_fillstyle("yellow");
	draw_circle(484, 22);
	
	set_fillstyle("black");
	c.font = "40px Arial";
    c.fillText(score,76,51);
    c.fillText(H,528,51);
    for(let i=0; i < n.length; i++){
		set_fillstyle(i == 0? "green" : "white");
		c.fillRect(n[i].x,n[i].y,j,j);
		c.strokeRect(n[i].x,n[i].y,j,j);
    }
	
	if(s>0&&s<5){d=s}
	set_fillstyle("red");
	draw_circle(j, 22);
	draw_circle(foodx[score], foody[score]);
	
    s = b*(d!=b-2&&d!=b+2);
	
	b = 0;
	
	if(d == 1){
		X-=j;
	}
	if(d == 2){
		Y-=j;
	}
	if(d == 3){
		X+=j;
	}
	if(d == 4){
		Y+=j;
	}
    if(!(X<foodx[score]||X>foodx[score]||Y>foody[score]||Y<foody[score])){
        score++;
		timer = 0;
		while(Collision(foodx[score],foody[score]) || (X == foodx[score] && Y == foody[score])){
			foodx[score] = Math.floor(Math.random()*17+1)*j;
			foody[score] = Math.floor(Math.random()*15+3)*j;
		}
		dstart = (Math.abs(X-foodx[score]) + Math.abs(Y-foody[score]))/32;
		dmin = dstart;
    }else{
        n.pop();
    }
	
	if(dmin > (Math.abs(X-foodx[score]) + Math.abs(Y-foody[score]))/32){
		dmin = (Math.abs(X-foodx[score]) + Math.abs(Y-foody[score]))/32;
	}
    
    if(Collision(X,Y) || X < j || X > 544 || Y < 96 || Y > 544 || timer > 1000 || deathkey == 1){
		fitness[att] += score + (dstart-dmin)/(2*dstart);
		timer = 0;
		deathkey = 0;
		d = 0;
		n = [];
		X = 288;
		Y = 320;
		n[0] = {x:X,y:Y};
		if(score > H) {
		H = score;
		}
		score = 0;
		for(let i = 0; i < 510; i++){
			value[0][i] = 0;
		}
		food();
		while(Collision(foodx[0],foody[0])){
			foodx[0] = Math.floor(Math.random()*17+1)*j;
			foody[0] = Math.floor(Math.random()*15+3)*j;
		}
		dstart = (Math.abs(X-foodx[0]) + Math.abs(Y-foody[0]))/32;
		dmin = dstart;
    }
	else{
		n.unshift({x:X,y:Y})
	}
}