#include "gradient.h"

extern HBRUSH	brBlue;
extern HBRUSH	brRed;

extern HPEN		pnRed;

bool IsNumeric(std::string num){
	bool eExist = false;
	bool dotExist = false;
	for(std::string::iterator i = num.begin(); i < num.end(); i++){
		switch((int)*i){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '.':
				if(!dotExist){
					dotExist = true;
				}else{
					return false;
				}
				break;
			case 'e':
				if(i == num.end() - 1 || eExist){	// e mo¿e wyst¹piæ tylko raz i w odleg³oœci conajmniej 1-go znaku od koñca
					return false;
				}else if(num.end() - 2 >= i){	// minus mo¿e wyst¹piæ po e tylko w odleg³oœci conajmniej 2-ch znaków od koñca np 1e-2 - poprawny 1e- - b³êdny
					if(*(i + 1) == '-')
						i++;
				}
				eExist = true;
				break;
			case '-':	// Gdy minus na pocz¹tku
				if(i != num.begin())
					return false;
				break;
			default:
				return false;
		}
	}
	return true;
}

gradient::gradient(void): width(1000), transparent(1000, HEIGHT), gr(1000, HEIGHT)
{
	for(UINT i = 0; i < width; i++){
		gTable.push_back(0);
		gTable.push_back(0);
		gTable.push_back(0);

		gATable.push_back(1);
	}
}

void gradient::operator =(const gradient &grad){
	width = grad.width;
	gTable = grad.gTable;
	gATable = grad.gATable;
	gname = grad.gname;
	transparent = grad.transparent;
	gr = grad.gr;
}

void gradient::SetGradient(std::vector<float> &v){
	if((v.size()>=2*4) && (v.size() % 4 == 0)){
		this->tSegment.clear();
		float dx = 1.f / (v.size() / 4 - 1);
		for(UINT i = 0; i < v.size() / 4 - 1; i++){
			tSegment.push_back(segment(sColor( v[i * 4], v[i * 4 + 1], v[i * 4 + 2], v[i * 4 + 3], i * dx), 
				sColor( v[(i + 1) * 4], v[(i + 1) * 4 + 1], v[(i + 1) * 4 + 2], v[(i + 1) * 4 + 3], (i + 1) * dx)));
		}
		CreateGradientVector();
	}
}

gradient::~gradient(void)
{
}

void gradient::SetWidth(UINT width){
	width = width;// - (width % 4);
	this->width = min(max(width,100),2000);

	transparent.SetWidth(this->width, HEIGHT);
	gr.SetWidth(this->width, HEIGHT);

	gTable.clear();
	gATable.clear();

	for(UINT i = 0; i < this->width; i++){
		gTable.push_back(0);
		gTable.push_back(0);
		gTable.push_back(0);

		gATable.push_back(1);
	}

	CreateGradientVector();
}

void gradient::CreateGradientVector(){
	float dk = 1.f / (width - 1);
	float p = 0;

	std::vector<BYTE>::iterator pt = gTable.begin();
	std::vector<float>::iterator pta = gATable.begin();
	
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		i->CreateGradientVector(p, dk, pt, pta);
	}

	int dw = gr.bmih.biWidth % 4;

	for(int i = 0; i < this->gr.bmih.biWidth; i++){
		for(int j = 0; j < this->gr.bmih.biHeight; j++){
			gr.pBits[(i + j * gr.bmih.biWidth) * 3 + j * dw] =		BYTE(this->gTable[i * 3] * gATable[i] + this->transparent.pBits[(i + j * gr.bmih.biWidth) * 3 + j * dw] * (1. - gATable[i]));
			gr.pBits[(i + j * gr.bmih.biWidth) * 3 + 1 + j * dw] =	BYTE(this->gTable[i * 3 + 1] * gATable[i] + this->transparent.pBits[(i + j * gr.bmih.biWidth) * 3 + 1 + j * dw] * (1. - gATable[i]));
			gr.pBits[(i + j * gr.bmih.biWidth) * 3 + 2 + j * dw] =	BYTE(this->gTable[i * 3 + 2] * gATable[i] + this->transparent.pBits[(i + j * gr.bmih.biWidth) * 3 + 2 + j * dw] * (1. - gATable[i]));
		}
	}
}

void gradient::LoadFile(HINSTANCE hInst){
	dlgOpenSaveFileName o(hInst, "pliki *.ggr\0*.ggr\0\0");
	if(o.ShowOpenFileName()){
		std::fstream file(o.GetFileName(),std::ios_base::in);
		std::string s;
        std::string name = o.GetFileName();

		std::basic_string <char>::size_type k = name.find_last_of('\\', std::string::npos);
		
		name.erase(0, k);

		if(file.fail())
			return ;
		std::getline(file, gname);
		
		if(gname.compare("GIMP Gradient")){
			file.close();
			return ;
		}
		if(file.fail())
			return ;
		std::getline(file, gname);
		
		if(gname.size() > 6u)
			gname.erase(0u, 6u);

		if(file.fail())
			return ;
		
		std::getline(file, s);

		int r = atol(s.c_str());

		if(!r){
			if(IsNumeric(gname)){
				r = atol(gname.c_str());
				tSegment.push_back(segment(s));
				gname = name;
			}
		}

		for(int i = 0; i < r; i++){
			if(file.fail())
				return ;
			std::getline(file, s);
			tSegment.push_back(segment(s));
		}

		CreateGradientVector();		

		file.close();
	}
}

void gradient::LoadFile(std::string path){
	std::fstream file(path.c_str(),std::ios_base::in);
    std::string s;
	
	std::string name = path;

	std::basic_string <char>::size_type k = name.find_last_of('\\', std::string::npos);
	
	name.erase(0, k);

	if(file.fail())
		return ;
	std::getline(file, gname);
	
	if(gname.compare("GIMP Gradient")){
		file.close();
		return ;
	}
	if(file.fail())
		return ;
	std::getline(file, gname);
	
	if(gname.size() > 6u)
		gname.erase(0u, 6u);

	if(file.fail())
		return ;

	tSegment.clear();
	
	std::getline(file, s);

	int r = atol(s.c_str());

	if(!r){
		if(IsNumeric(gname)){
			r = atol(gname.c_str());
			tSegment.push_back(segment(s));
			gname = name;
		}
	}

	for(int i = 0; i < r; i++){
		if(file.fail())
			return ;
		std::getline(file, s);
		tSegment.push_back(segment(s));
	}

	CreateGradientVector();		

	file.close();
}

void gradient::GetPointColor(int index, float &rl, float &gl, float &bl, float &al, float &rr, float &gr, float &br, float &ar){
	if(index > -1 && index <= (int)tSegment.size()){
		if(index == 0){
			tSegment[0].GetLeftColorRGB(rl, gl, bl, al);
			rr = rl; gr = gl; br = bl; ar = al;
		}else if(index == (int)tSegment.size()){
			(tSegment.end()-1)->GetRightColorRGB(rr, gr, br, ar);
			rl = rr; gl = gr; bl = br; al = ar;
		}else{
			tSegment[index].GetLeftColorRGB(rr, gr, br, ar);
			tSegment[index - 1].GetRightColorRGB(rl, gl, bl, al);
		}
	}
}

void gradient::DrawGradientPt(HDC &hdc, POINT fp, POINT lp){
	POINT pt;
	POINT dp = lp - fp;
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end() - 1; i++){
		pt = i->SegmentOnScreen(fp,dp);
		Ellipse(hdc,pt.x - H_SIZE,pt.y - H_SIZE,pt.x + H_SIZE,pt.y + H_SIZE);
	}
}

void gradient::DrawGrPt(HDC &hdc, UINT width, UINT height,int grPtInd,int grSgInd){
	if(tSegment.size()){
		POINT pt;
		pt.y = height / 2;
		if(grSgInd > -1 && grSgInd < (int)tSegment.size() && grPtInd == -1){
			RECT r;
			SetRect(&r, int(tSegment[grSgInd].left.pos * width), 0, int(tSegment[grSgInd].right.pos * width), height);
			FillRect(hdc, &r, grSgInd == 0 ? brRed : (grSgInd == tSegment.size() - 1 ? brRed : brBlue));
		}
		for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
			pt.x = i->SegmentOnScreen(width);
			Ellipse(hdc, pt.x - H_SIZE,pt.y - H_SIZE, pt.x + H_SIZE,pt.y + H_SIZE);
			if(i == tSegment.begin()){
				Ellipse(hdc, - H_SIZE,pt.y - H_SIZE, H_SIZE,pt.y + H_SIZE);
			}
		}
		if(grPtInd > -1 && grPtInd < (int)tSegment.size() - 1){
			int x =  tSegment[grPtInd].SegmentOnScreen(width);
			SelectObject(hdc, pnRed);
			Ellipse(hdc, x - H_SIZE, pt.y - H_SIZE, x + H_SIZE, pt.y + H_SIZE);
		}
	}
}

void gradient::AddPoint(float r,float g,float b,float a,float pos){
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		if(i->left.pos < pos && i->right.pos > pos){
			segment s(i->left,sColor(r, g, b, a, pos));
			*i = segment(s.right, i->right);
			tSegment.insert(i,s);
			CreateGradientVector();
			break;
		}
	}
}

void gradient::SetGradientColors(POINT fp, POINT dp, BYTE* table, int pitch){
	POINT p1;
	POINT p2;
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		p1 = fp + dp * i->left.pos;
		p2 = fp + dp * i->right.pos;
		*i = segment(sColor( (float)table[p1.x * 3 + p1.y * pitch + 2] / 255.f, (float)table[p1.x * 3 + p1.y * pitch + 1] / 255.f, (float)table[p1.x * 3 + p1.y * pitch] / 255.f,1.f,i->left.pos),
				sColor( (float)table[p2.x * 3 + p2.y * pitch + 2] / 255.f, (float)table[p2.x * 3 + p2.y * pitch + 1] / 255.f, (float) table[p2.x * 3 + p2.y * pitch] / 255.f,1.f,i->right.pos));
	}
	CreateGradientVector();
}

void gradient::SetGradientColors32(POINT fp, POINT dp, BYTE* table, int pitch){
	POINT p1;
	POINT p2;
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		p1 = fp + dp * i->left.pos;
		p2 = fp + dp * i->right.pos;
		*i = segment(sColor( (float)table[p1.x * 4 + p1.y * pitch + 2] / 255.f, (float)table[p1.x * 4 + p1.y * pitch + 1] / 255.f, (float)table[p1.x * 4 + p1.y * pitch] / 255.f,(float) table[p1.x * 4 + p1.y * pitch + 3] / 255.f,i->left.pos),
				sColor( (float)table[p2.x * 4 + p2.y * pitch + 2] / 255.f, (float)table[p2.x * 4 + p2.y * pitch + 1] / 255.f, (float) table[p2.x * 4 + p2.y * pitch] / 255.f,(float) table[p2.x * 4 + p2.y * pitch + 3] / 255.f,i->right.pos));
	}
	CreateGradientVector();
}

int gradient::MouseInPt(POINT mp,POINT fp,POINT dp, double scale){
	POINT p1;
	POINT p2;
	scale = H_SIZE * H_SIZE / scale;
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		p1 = fp + dp * i->left.pos;
		p2 = fp + dp * i->right.pos;
		if((p1 - mp) * (p1 - mp) <= scale){
			return (int)(i - tSegment.begin());
		}
		if((p2 - mp) * (p2 - mp) <= scale){
			return (int)(i - tSegment.begin()) + 1;
		}
	}
	return -1;
}

void gradient::FindGrPtAndSegmentIndex(int &grPtInd, int &grSgInd,int x,int width,int ray){
	grSgInd = grPtInd = -1;
	if(grSgInd == -1 && tSegment.size()){
		std::vector<segment>::iterator i = tSegment.begin();
		if(i->left.pos * width - ray < x && i->left.pos * width + ray > x){
			grPtInd = -2;
			return ;
		}
		i = tSegment.end() - 1;
		if(i->right.pos * width - ray < x && i->right.pos * width + ray > x){
			grPtInd = -3;
			return ;
		}
	}
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		if(i->right.pos * width > x && x > i->left.pos * width){
			grSgInd = (int)(i - tSegment.begin());
			if(grPtInd != -1)
				return;
		}
		if(i->right.pos * width - ray < x && i->right.pos * width + ray > x && i != tSegment.end() - 1){
			grPtInd = (int)(i - tSegment.begin());
			if(grSgInd != -1)
				return;
		}
	}
}

void gradient::DeletePoint(int selPt){
	if(selPt && selPt < (int)tSegment.size()){
		tSegment[selPt - 1] = segment(tSegment[selPt - 1].left, tSegment[selPt].right);
		tSegment.erase(tSegment.begin() + selPt);
		CreateGradientVector();
	}
}

std::string gradient::GradientToString(){
	std::string s;
	for(std::vector<segment>::iterator i = tSegment.begin(); i < tSegment.end(); i++){
		s += i->SegmentToString(i == tSegment.end() - 1 ? true:false);
	}
	return s;
}

std::string RemSign(std::string s, const char* sign){
	std::basic_string <char>::size_type i = s.find_first_of(sign, 0);
	while(i != std::string::npos){
		s.erase(i, 1);
		i = s.find_first_of(sign, 0);
	}
	return s;
}

char filtr[] = " #&";

std::string gradient::GetSvgGradientString(std::string idS){
	std::string s;

	if(!tSegment.size())
		return s;

	s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<!-- Created with Inkscape (http://www.inkscape.org/) -->\n\n";
	s += "<svg";
	s += char(10);
	s += "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"";
	s += char(10);
	s += "xmlns:cc=\"http://creativecommons.org/ns#\"";
	s += char(10);
	s += "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"";
	s += char(10);
	s += "xmlns:svg=\"http://www.w3.org/2000/svg\"";
	s += char(10);
	s += "xmlns=\"http://www.w3.org/2000/svg\"";
	s += char(10);
	s += "xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
	s += char(10);
	s += "xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"";
	s += char(10);
	s += "xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"";
	s += char(10);
	s += "width=\"744.09448819\"";
	s += char(10);
	s += "height=\"1052.3622047\"";
	s += char(10);
	s += "id=\"svg2\"";
	s += char(10);
	s += "version=\"1.1\"";
	s += char(10);
	s += "inkscape:version=\"0.48.0 r9654\"";
	s += char(10);
	s += "sodipodi:docname=\"Nowy dokument 1\">";
	s += "<defs";
	s += char(10);
	s += "id=\"defs4\">";
	s += char(10);

	s += "<linearGradient";
	s += char(10);
	s += "inkscape:collect=\"always\"";
	s += char(10);
	s += "id=\"";
	s += this->gname.size() ? RemSign(gname,filtr) : RemSign(idS,filtr);
	s += "\">";
	s += char(10);

	int id = 0;

	s += tSegment[0].left.GetSvgString(id);
	for(int i = 0; i < tSegment.size() - 1; i++){
		s += tSegment[i].right.GetSvgString(id);
		if(!(tSegment[i].right == tSegment[i + 1].left)){
			s += tSegment[i + 1].left.GetSvgString(id);
		}
	}
	s += (tSegment.end()-1)->right.GetSvgString(id);

	s += "</linearGradient>";
	s += char(10);

	s += char(10);
	s += "</defs>";
	s += char(10);

	s += "<sodipodi:namedview";
	s += char(10);
	s += "id=\"base\"";
	s += char(10);
	s += "pagecolor=\"#ffffff\"";
	s += char(10);
	s += "bordercolor=\"#666666\"";
	s += char(10);
	s += "borderopacity=\"1.0\"";
	s += char(10);
	s += "inkscape:pageopacity=\"0.0\"";
	s += char(10);
	s += "inkscape:pageshadow=\"2\"";
	s += char(10);
	s += "inkscape:zoom=\"0.98994949\"";
	s += char(10);
	s += "inkscape:cx=\"210.46193\"";
	s += char(10);
	s += "inkscape:cy=\"780.98887\"";
	s += char(10);
	s += "inkscape:document-units=\"px\"";
	s += char(10);
	s += "inkscape:current-layer=\"layer1\"";
	s += char(10);
	s += "showgrid=\"false\"";
	s += char(10);
	s += "inkscape:window-width=\"1440\"";
	s += char(10);
	s += "inkscape:window-height=\"820\"";
	s += char(10);
	s += "inkscape:window-x=\"-8\"";
	s += char(10);
	s += "inkscape:window-y=\"-8\"";
	s += char(10);
	s += "inkscape:window-maximized=\"1\" />";
	s += char(10);

	s += "<metadata";
	s += char(10);
	s += "id=\"metadata7\">";

	s += "<rdf:RDF>";
	s += "<cc:Work rdf:about=\"\">";
    s += "<dc:format>image/svg+xml</dc:format>";
    s += "<dc:type rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />";
	s += "<dc:title />";
	s += "</cc:Work>";
    s += "</rdf:RDF>";
	s += "</metadata>";
	s += "<g inkscape:label=\"Layer 1\" inkscape:groupmode=\"layer\" id=\"layer1\">";
	s += "<rect style=\"color:#000000;fill:url(#";
	s += this->gname.size() ? RemSign(gname,filtr) : RemSign(idS,filtr);
	s += ");fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:2;marker:none;visibility:visible;display:inline;overflow:visible;enable-background:accumulate\" id=\"rect2985\" width=\"100\" height=\"100\" x=\"0\" y=\"0\" />";
	s += "</g>";
	s += "</svg>";

	return s;
}

std::string gradient::GetSvgGradientDefinition(std::string idS){
	std::string s;

	if(!tSegment.size())
		return s;

	s = "<linearGradient";
	s += char(10);
	s += "inkscape:collect=\"always\"";
	s += char(10);
	s += "id=\"";
	s += this->gname.size() ? RemSign(gname,filtr) : RemSign(idS,filtr);
	s += "\">";
	s += char(10);

	int id = 0;

	s += tSegment[0].left.GetSvgString(id);
	for(int i = 0; i < tSegment.size() - 1; i++){
		s += tSegment[i].right.GetSvgString(id);
		if(!(tSegment[i].right == tSegment[i + 1].left)){
			s += tSegment[i + 1].left.GetSvgString(id);
		}
	}
	s += (tSegment.end()-1)->right.GetSvgString(id);

	s += "</linearGradient>";
	s += char(10);

	return s;
}

std::string gradient::GetSvgGradientObject(std::string idS, int idR, int x, int y, int height, int width){
	std::string s;

	if(!tSegment.size())
		return s;

	char c[100];

	sprintf(c, "%i", idR);

	s = "<rect";
	s += char(10);
	s += "style=\"color:#000000;fill:url(#";
	s += this->gname.size() ? RemSign(gname,filtr) : RemSign(idS,filtr);
	s += ");fill-opacity:1;fill-rule:nonzero;stroke:none;stroke-width:2;marker:none;visibility:visible;display:inline;overflow:visible;enable-background:accumulate\" id=\"rect";
	s += c;
	s += "\" width=\"";
	sprintf(c, "%i", width);
	s += c;
	s += "\" height=\"";
	sprintf(c, "%i", height);
	s += c;
	s += "\" x=\"";
	sprintf(c, "%i", x);
	s += c;
	s += "\" y=\"";
	sprintf(c, "%i", y);
	s += c;
	s += "\" />";
	s += char(10);

	return s;
}

std::string gradient::GetHtmlGradientDef(std::string path){
	std::string s;

	std::basic_string <char>::size_type i = path.find_last_of('\\', std::string::npos);

	if(i != std::string::npos){
		path.erase(0, i + 1);
	}

	s += "<h2 class=\"";
	s += RemSign(this->gname,filtr);
	s+= "\">";
	s += "<a href=\"";
	s += path;
	s += "\">";
	s += gname;
	s += "</a>";
	s += "</h2>\n";
	return s;
}

std::string gradient::GetCssGradientString(){
	std::string s;

	if(!tSegment.size())
		return s;

	char c[100];

	std::string gm;
	std::string chr;
	std::string gn = RemSign(this->gname,filtr);

	if(gn.size()){
		s += "h2.";

		s += gn;
	}else{
		s += "h2";
	}
	s += "\n{\n";

	gm += "linear-gradient(left, ";
	chr += "linear, left top, right top, color-stop( 0\%, ";

	gm+= tSegment[0].left.ChangeColorToRGBAString();
	chr += tSegment[0].left.ChangeColorToRGBAString();
	gm+= " 0\%, ";
	chr += "), color-stop(";

	for(int i = 1; i < tSegment.size(); i++){
		gm+= tSegment[i-1].right.ChangeColorToRGBAString();
		
		ltoa(myMath::Zaokr(tSegment[i-1].right.pos * 100), c, 10);
		gm+= " ";
		gm+= c;
		gm+= "\%, ";
		chr += c;
		chr += "\%, ";
		chr += tSegment[i-1].right.ChangeColorToRGBAString();
		chr += "), color-stop(";
		if(!(tSegment[i-1].right == tSegment[i].left)){
			gm+= tSegment[i].left.ChangeColorToRGBAString();
			gm+= " ";
			gm+= c;
			gm+= "\%, ";
			chr += c;
			chr += "\%, ";
			chr += tSegment[i-1].right.ChangeColorToRGBAString();
			chr += "), color-stop(";
		}
	}
	gm+= tSegment[tSegment.size() - 1].right.ChangeColorToRGBAString();
	gm+= " 100\%);";
	chr += "100%, ";
	chr += tSegment[tSegment.size() - 1].right.ChangeColorToRGBAString();
	chr += "));";

	s+= "\tbackground-image: ";
	s+= gm;
	s+= "\n\tbackground-image: -moz-";
	s+= gm;
	s+= "\n\tbackground-image: -o-";
	s+= gm;
	s+= "\n\tbackground-image: -webkit-";
	s+= gm;
	s+= "\n\tbackground-image: -ms-";
	s+= gm;
	s+= "\n\tbackground-image: -webkit-gradient(";
	s+= chr;
	s+= "\n}\n";

	return s;
}

void gradient::GradientFromXml(c_xml &xml,int index){
	std::vector<sColor> tColors;

	if(index < 0 || index >= xml.xmlT.size())
		return;

	for(int i = 0; i < xml.xmlT[index].xmlT.size(); i++){
		tColors.push_back(sColor(xml.xmlT[index].xmlT[i]));
	}

	if(tColors.size() < 2)
		return;

	tSegment.clear();

	this->gname = xml.xmlT[index].tVal[xml.xmlT[index].FindValueIndex("id")].value;

	for(std::vector<sColor>::iterator i = tColors.begin(); i < tColors.end() - 1; i++){
		tSegment.push_back(segment(*i, *(i + 1)));
	}

	CreateGradientVector();

	//std::string buffor("GIMP Gradient");
	//buffor += char(10u);
	//buffor += "Name: ";
	//int iID = xml.xmlT[index].FindValueIndex("id");
	//buffor += xml.xmlT[index].tVal[iID].value;
	//buffor += char(10u);
	//char buf[200];
	//sprintf(buf, "%i",tPt.size() - 1);
	//buffor += buf;
	//buffor += char(10u);

	//if(tPt.size() - 1 == 1){
	//	sprintf(buf,"%f %f %f %f %f %f %f %f %f %f %f 0 0%c",tPt[0].offset,(tPt[0].offset + tPt[1].offset) / 2.,tPt[1].offset,tPt[0].red,tPt[0].green,tPt[0].blue, tPt[0].alpha, tPt[1].red, tPt[1].green, tPt[1].blue, tPt[1].alpha, char(10));
	//	buffor += buf;
	//}else{
	//	for(int i = 0; i < tPt.size() - 1; i++){
	//		sprintf(buf,"%f %f %f %f %f %f %f %f %f %f %f 0 0 0 0%c",tPt[i].offset,(tPt[i].offset + tPt[i + 1].offset) / 2.,tPt[i + 1].offset,tPt[i].red,tPt[i].green,tPt[i].blue, tPt[i].alpha, tPt[i+1].red, tPt[i+1].green, tPt[i+1].blue, tPt[i+1].alpha, char(10));
	//		buffor += buf;
	//	}
	//}
	//
	//std::string location = xml.xmlT[index].tVal[iID].value;

	//location += ".ggr";

	//std::fstream file(location.c_str(),std::ios_base::out);

	//file.write(buffor.c_str(),buffor.size());
	//file.close();
	
	//std::cout<<std::endl<<"Utworzono plik o nazwie (Create file with name): "<<xml.xmlT[index].tVal[iID].value.c_str();
}

void gradient::MoveSegment(float dx,int index){
	if(index < 1 || index > (int)tSegment.size() - 1)
		return;
	if(dx > 0){
		dx = min(tSegment[index + 1].right.pos - tSegment[index].right.pos, dx);
	}else if(dx < 0){
		dx = max(tSegment[index - 1].left.pos -  tSegment[index].left.pos, dx);
	}
	tSegment[index].left.pos += dx;
	tSegment[index].center.pos += dx;
	tSegment[index].right.pos += dx;
	tSegment[index + 1].SetLeftPos(tSegment[index + 1].left.pos + dx);
	tSegment[index - 1].SetRightPos(tSegment[index - 1].right.pos + dx);
}