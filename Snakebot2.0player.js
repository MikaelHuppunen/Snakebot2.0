let c = document.getElementById("s").getContext("2d"),j=32, X = 288,Y = 320, 
score = 0,H = 0,s,b,d, m=Math.floor, gen = 0, att = 0, f, F, 
crossoverpoint, timer = 0, totalfit, foodx = [], foody = [], dmin = 15, n = [{x:X,y:Y}], gensize = 200,
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
		foodx[i] = m(Math.random()*17+1) * j;
		foody[i] = m(Math.random()*15+3) * j;
	}
}
food();

f = foodx[0];
F = foody[0];
dstart = (Math.abs(X-f) + Math.abs(Y-F))/32;
dmin = dstart;
while(Collision(f,F)){
	f = m(Math.random()*17+1)*j;
	F = m(Math.random()*15+3)*j;
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
	value[0][512] = ((f-32)/32)/17;
	value[0][513] = ((F-96)/32)/15;
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

function e(a, b){
	c.beginPath();
	c.arc(a + 16, b + 16, 16, 0, 7);
	c.fill()
}

function l(a){
	c.fillStyle = a
}


function D(){
	timer++;
	ai();
	l("#4a752c");
	c.fillRect(0, 0, 608, 72);
	l("#578a34");
	c.fillRect(0, 72, 608, 536);
	l("#aad751");
	c.fillRect(j, 96, 544, 480);
	c.rect(j, 0, 576, 576);
	c.clip();
	for(let i = 999; i > 0; i--){
		l("#a2d149");
		c.fillRect((2*i-m(i/9)*18+m(i/9)%2)*j, m(i/9)*j+96, j, j)
	}
	l("yellow");
	e(484, 22);
	
	l("black");
	c.font = "40px Arial";
    c.fillText(score,76,51);
    c.fillText(H,528,51);
    for(let i=0; i < n.length; i++){
		l(i == 0? "green" : "white");
		c.fillRect(n[i].x,n[i].y,j,j);
		c.strokeRect(n[i].x,n[i].y,j,j);
    }
	
	if(s>0&&s<5){d=s}
	l("red");
	e(j, 22);
	e(f, F);
	
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
    if(!(X<f||X>f||Y>F||Y<F)){
        score++;
		timer = 0;
		f = foodx[score];
		F = foody[score];
		while(Collision(f,F) || (X == f && Y == F)){
			f = m(Math.random()*17+1)*j;
			F = m(Math.random()*15+3)*j;
		}
		dstart = (Math.abs(X-f) + Math.abs(Y-F))/32;
		dmin = dstart;
    }else{
        n.pop();
    }
	
	if(dmin > (Math.abs(X-f) + Math.abs(Y-F))/32){
		dmin = (Math.abs(X-f) + Math.abs(Y-F))/32;
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
		f = foodx[0];
		F = foody[0];
		while(Collision(f,F)){
			f = m(Math.random()*17+1)*j;
			F = m(Math.random()*15+3)*j;
		}
		dstart = (Math.abs(X-f) + Math.abs(Y-F))/32;
		dmin = dstart;
    }
	else{
		n.unshift({x:X,y:Y})
	}
}