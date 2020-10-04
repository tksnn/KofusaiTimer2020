
#include <Siv3D.hpp>
#include <map>
#include "TimerS.h"
#include "GetIPAddress.h"
#include "HTTPClient.hpp"

enum class Scenes {
	StartUp,
	Main
};

enum Status {
	Disconnect,
	Waiting,
	Playing
};

void Main()
{
	//Window
	Window::Resize(1100, 640);
	Window::SetTitle(U"Timer");
	Scene::SetBackground(Palette::White);
	//Assets
	{
		FontAsset::Register(U"SmartFont40", 40, U"Assets/Font/03SmartFontUI.ttf", AssetParameter::LoadAsync());
		FontAsset::Register(U"SmartFont25", 25, U"Assets/Font/03SmartFontUI.ttf", AssetParameter::LoadAsync());
		FontAsset::Register(U"SmartFont30", 30, U"Assets/Font/03SmartFontUI.ttf", AssetParameter::LoadAsync());
		FontAsset::Register(U"SmartFont15", 15, U"Assets/Font/03SmartFontUI.ttf", AssetParameter::LoadAsync());
		FontAsset::Register(U"KakkutoFont40", 40, U"Assets/Font/851H-kktt_004.ttf", AssetParameter::LoadAsync());
		TextureAsset::Register(U"logo", U"Assets/logo.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"chu", U"Assets/chu.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"yajirusi_d", U"Assets/yajirusi_d.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"yajirusi_u", U"Assets/yajirusi_u.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"r", U"Assets/r.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"s", U"Assets/s.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"st", U"Assets/st.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"ar", U"Assets/ar.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"as", U"Assets/as.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"ast", U"Assets/ast.png", AssetParameter::LoadAsync());
		for (int i = 1; i <= 12; i++) {
			AudioAsset::Register(Format(U"v", i), Format(U"Assets/Audio/v", i, U".mp3"), AssetParameter::LoadAsync());
		}
	}
	//MainCongig
	Scenes scene = Scenes::StartUp;
	Font title_font(40);
	//TCP
	TCPServer server;
	IPtable table;
	std::string ip = table.getIPAddress();
	if (!HTTPClient::InitCURL()) return;
	HTTPClient client;
	const URL sendipurl = U"https://negima-p.work/api/v1/pcp/saveip.php?ip=" + Unicode::Widen(ip);
	const FilePath localFilePathresult = U"result.data";
	if (!client.downloadFile(sendipurl, localFilePathresult)) return;
	if (scene == Scenes::StartUp) {
		bool loaded = false;
		String message = U"起動中...";
		Stopwatch sw;
		while (System::Update()) {
			const double h = Periodic::Sine0_1(2.5s);
			if (!loaded) {
				message = U"アセットをロード中...";
				const bool rf1 = FontAsset::IsReady(U"SmartFont40");
				const bool rf2 = FontAsset::IsReady(U"KakkutoFont40");
				const bool rf3 = FontAsset::IsReady(U"SmartFont25");
				const bool rf4 = FontAsset::IsReady(U"SmartFont30");
				const bool rf5 = FontAsset::IsReady(U"SmartFont15");
				const bool rt1 = TextureAsset::IsReady(U"logo");
				const bool rt2 = TextureAsset::IsReady(U"chu");
				const bool rt3 = TextureAsset::IsReady(U"yajirusi_d");
				const bool rt4 = TextureAsset::IsReady(U"yajirusi_u");
				const bool rt5 = TextureAsset::IsReady(U"r");
				const bool rt6 = TextureAsset::IsReady(U"s");
				const bool rt7 = TextureAsset::IsReady(U"st");
				const bool rt8 = TextureAsset::IsReady(U"ar");
				const bool rt9 = TextureAsset::IsReady(U"as");
				const bool rt10 = TextureAsset::IsReady(U"ast");
				bool ra = true;
				for (int i = 1; i <= 12; i++) {
					ra = AudioAsset::IsReady(Format(U"v", i));
					if (!ra) break;
				}
				if (rf1 && rf2 && rf3 && rf4 && rf5 && rt1 && rt2 && rt3 && rt4 && rt5 && rt6 && rt7 && rt8 && rt9 && rt10 && ra) {
					loaded = true;
					sw.start();
				}
				if (rt2) {
					TextureAsset(U"chu").scaled(0.25).drawAt(540, 290, ColorF(1, 1, 1, h));
				}
				title_font(message).drawAt(540, 580, Palette::Black);
			}
			else {
				if (sw.s() >= 3) {
					message = U"Zを押して開始";
					if (KeyZ.down()) {
						scene = Scenes::Main;
						break;
					}
				}
				else {
					message = U"起動中...";
				}
				FontAsset(U"SmartFont40")(message).drawAt(540, 580, Palette::Black);
				TextureAsset(U"chu").scaled(0.25).drawAt(540, 290, ColorF(1, 1, 1, h));
			}
		}
	}
	if (scene == Scenes::Main) {
		int ranjyoninzu = 0;
		std::map<String, int> gamebetu;
		String gamebetustr = U"";
		//TCP
		server.startAcceptMulti(51126);
		String sendipresult = TextReader(localFilePathresult).readAll();
		std::map<int, size_t> TCPSessions = {
			{1, NULL},{2, NULL},{3, NULL},{4, NULL},{5, NULL},{6, NULL},{7, NULL},{8, NULL},{9, NULL},{10, NULL},{11, NULL},{12, NULL},
		};
		//Wedge
		Array<String> StatusS = {
			U"Failing",
			U"Waiting",
			U"Playing"
		};
		Seconds limit_s = 300s;
		String limit_forsend = Format(300);
		Rect r1 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(100, 190));
		Rect rs1 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(100, 230));
		Rect rr1 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(100, 270));
		Rect r2 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(250, 190));
		Rect rs2 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(250, 230));
		Rect rr2 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(250, 270));
		Rect r3 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(400, 190));
		Rect rs3 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(400, 230));
		Rect rr3 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(400, 270));
		Rect r4 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(550, 190));
		Rect rs4 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(550, 230));
		Rect rr4 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(550, 270));
		Rect r5 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(700, 190));
		Rect rs5 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(700, 230));
		Rect rr5 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(700, 270));
		Rect r6 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(850, 190));
		Rect rs6 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(850, 230));
		Rect rr6 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(850, 270));
		Rect r7 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(100, 440));
		Rect rs7 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(100, 480));
		Rect rr7 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(100, 520));
		Rect r8 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(250, 440));
		Rect rs8 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(250, 480));
		Rect rr8 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(250, 520));
		Rect r9 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(400, 440));
		Rect rs9 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(400, 480));
		Rect rr9 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(400, 520));
		Rect r10 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(550, 440));
		Rect rs10 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(550, 480));
		Rect rr10 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(550, 520));
		Rect r11 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(700, 440));
		Rect rs11 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(700, 480));
		Rect rr11 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(700, 520));
		Rect r12 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(850, 440));
		Rect rs12 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(850, 480));
		Rect rr12 = FontAsset(U"SmartFont30")(U"  Start ").regionAt(Point(850, 520));
		Array<Rect> r = { r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12 };
		Array<Rect> rs = { rs1, rs2, rs3, rs4, rs5, rs6, rs7, rs8, rs9, rs10, rs11, rs12 };
		Array<Rect> rr = { rr1, rr2, rr3, rr4, rr5, rr6, rr7, rr8, rr9, rr10, rr11, rr12 };
		Rect allr = FontAsset(U"SmartFont30")(U"AllStart ").regionAt(Point(100, 580));
		Rect allrs = FontAsset(U"SmartFont30")(U"AllStart ").regionAt(Point(250, 580));
		Rect allrr = FontAsset(U"SmartFont30")(U"AllStart ").regionAt(Point(400, 580));
		Rect upbtn = FontAsset(U"SmartFont30")(U"↑").regionAt(Point(600, 580));
		Rect dwnbtn = FontAsset(U"SmartFont30")(U"↓").regionAt(Point(750, 580));
		//Quit
		Array<TimerS> ts = { TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s),TimerS(limit_s) };
		Array<Status> statuses = { Disconnect, Disconnect, Disconnect, Disconnect, Disconnect, Disconnect ,Disconnect, Disconnect, Disconnect, Disconnect, Disconnect, Disconnect };
		Array<bool> pwstatuss = { false, false, false, false, false, false, false, false, false, false, false, false };
		while (System::Update()) {
			//TCP
			for (int i = 0; i < 12; i++) {
				bool eee = false;
				for (int m = 0; m < server.getSessionIDs().size(); m++) {
					size_t im = server.getSessionIDs()[m];
					if (TCPSessions[i + 1] == im) {
						eee = true;
						break;
					}
				}
				if (!eee) {
					if (statuses[i] == Waiting || statuses[i] == Playing) {
						//ts[i].pause();
						ts[i].reset();
					}
					TCPSessions[i + 1] = NULL;
					statuses[i] = Disconnect;
				}
				if (statuses[i] == Playing && TCPSessions[i + 1] != NULL) {
					if (!pwstatuss[i])pwstatuss[i] = true;
					if (ts[i].reachedZero()) {
						if (!ts[i].ex_flag) {
							const String Str = U"Atn:Rcz\n";
							const std::string str = Str.toUTF8();
							server.send(str.data(), sizeof(char) * str.length(), TCPSessions[i + 1]);
							AudioAsset(Format(U"v", (i + 1))).playOneShot(0.8);
							ts[i].ex_flag = true;
						}
					}
				}
				if (statuses[i] == Waiting && TCPSessions[i + 1] != NULL) {
					const String Str = Format(U"Lmt:", limit_forsend, U"\n");
					const std::string str = Str.toUTF8();
					server.send(str.data(), sizeof(char) * str.length(), TCPSessions[i + 1]);
				}
			}
			for (int m = 0; m < server.getSessionIDs().size(); m++) {
				size_t i = server.getSessionIDs()[m];
				//Print(i, Palette::Black);
				std::string message = "";
				while (true) {
					char ch;
					if (!server.read(ch, i)) break;
					if (ch == '\n') break;
					else message.push_back(ch);
				}
				if (message != "") {
					String mes = Unicode::FromUTF8(message);
					String command = mes.substr(0, 3);
					String text = mes.substr(4);
					if (command == U"Rgs") {
						ranjyoninzu++;
						TCPSessions[Parse<int>(text)] = i;
						statuses[Parse<int>(text) - 1] = Waiting;
						ts[Parse<int>(text) - 1].reset();
					}
					if (command == U"Srt") {
						statuses[Parse<int>(text) - 1] = Playing;
						ts[Parse<int>(text) - 1].start();
					}
					if (command == U"Ply") {
						gamebetu[text] += 1;
					}
					if (command == U"End") {
						statuses[Parse<int>(text) - 1] = Waiting;
						ts[Parse<int>(text) - 1].reset();
					}
				}
			}
			for (int i = 1; i <= 12; i++) {
				if (statuses[i - 1] == Disconnect && ts[i - 1].reachedZero() && !ts[i - 1].ex_flag) {
					ts[i - 1].ex_flag = true;
					AudioAsset(Format(U"v" , i)).playOneShot(0.8);
				}
			}
			FontAsset(U"SmartFont15")(Unicode::Widen(ip), U":", server.port(), U" Connection:", server.num_sessions()).draw(1060 - FontAsset(U"SmartFont15")(Unicode::Widen(ip), U":", server.port(), U" Connection:", server.num_sessions()).region(0, 0).w, 620, ColorF(Palette::Black, 0.8));
			FontAsset(U"SmartFont15")(U"CPU: ", CPU::GetFeature().name, U"Network: ", sendipresult).draw(10, 620, ColorF(Palette::Black, 0.8));
			//title
			TextureAsset(U"chu").scaled(0.05).draw(10, 10);
			FontAsset(U"SmartFont40")(U"2020年興風祭").draw(60, 10, Palette::Black);
			DateTime time = DateTime::Now();
			FontAsset(U"SmartFont40")(time.format(U"MM/dd-HH:mm")).draw(790, 9, Palette::Black);
			int xminad = static_cast<int>(FontAsset(U"SmartFont40")(time.format(U"MM/dd-HH:mm")).region(790, 9).w) + 790;
			FontAsset(U"SmartFont25")(time.format(U":ss")).draw(xminad, 10, Palette::Black);
			//1
			{
				FontAsset(U"SmartFont15")(U"No.1").drawAt(60, 83, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[0]]).drawAt(100, 110, Palette::Black);
				FontAsset(U"SmartFont30")(ts[0].mmss()).drawAt(100, 150, ts[0].reachedZero() ? Palette::Red : Palette::Black);
				if (r1.leftPressed()) r1.drawFrame(-1, Palette::Gray);
				else r1.drawFrame(-1, Palette::Black);
				if (r1.leftClicked()) ts[0].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(100, 190, Palette::Black);
				if (rs1.leftPressed()) rs1.drawFrame(-1, Palette::Gray);
				else rs1.drawFrame(-1, Palette::Black);
				if (rs1.leftClicked()) ts[0].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(100, 230, Palette::Black);
				if (rr1.leftPressed()) rr1.drawFrame(-1, Palette::Gray);
				else rr1.drawFrame(-1, Palette::Black);
				if (rr1.leftClicked()) ts[0].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(100, 270, Palette::Black);
			}
			//2
			{
				FontAsset(U"SmartFont15")(U"No.2").drawAt(210, 83, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[1]]).drawAt(250, 110, ts[1].reachedZero() ? Palette::Red : Palette::Black);
				FontAsset(U"SmartFont30")(ts[1].mmss()).drawAt(250, 150, Palette::Black);
				if (r2.leftPressed()) r2.drawFrame(-1, Palette::Gray);
				else r2.drawFrame(-1, Palette::Black);
				if (r2.leftClicked()) ts[1].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(250, 190, Palette::Black);
				if (rs2.leftPressed()) rs2.drawFrame(-1, Palette::Gray);
				else rs2.drawFrame(-1, Palette::Black);
				if (rs2.leftClicked()) ts[1].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(250, 230, Palette::Black);
				if (rr2.leftPressed()) rr2.drawFrame(-1, Palette::Gray);
				else rr2.drawFrame(-1, Palette::Black);
				if (rr2.leftClicked()) ts[1].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(250, 270, Palette::Black);
			}
			//3
			{
				FontAsset(U"SmartFont15")(U"No.3").drawAt(360, 83, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[2]]).drawAt(400, 110, ts[2].reachedZero() ? Palette::Red : Palette::Black);
				FontAsset(U"SmartFont30")(ts[2].mmss()).drawAt(400, 150, Palette::Black);
				if (r3.leftPressed()) r3.drawFrame(-1, Palette::Gray);
				else r3.drawFrame(-1, Palette::Black);
				if (r3.leftClicked()) ts[2].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(400, 190, Palette::Black);
				if (rs3.leftPressed()) rs3.drawFrame(-1, Palette::Gray);
				else rs3.drawFrame(-1, Palette::Black);
				if (rs3.leftClicked()) ts[2].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(400, 230, Palette::Black);
				if (rr3.leftPressed()) rr3.drawFrame(-1, Palette::Gray);
				else rr3.drawFrame(-1, Palette::Black);
				if (rr3.leftClicked()) ts[2].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(400, 270, Palette::Black);
			}
			//4
			{
				FontAsset(U"SmartFont15")(U"No.4").drawAt(510, 83, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[3]]).drawAt(550, 110, ts[3].reachedZero() ? Palette::Red : Palette::Black);
				FontAsset(U"SmartFont30")(ts[3].mmss()).drawAt(550, 150, Palette::Black);
				if (r4.leftPressed()) r4.drawFrame(-1, Palette::Gray);
				else r4.drawFrame(-1, Palette::Black);
				if (r4.leftClicked()) ts[3].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(550, 190, Palette::Black);
				if (rs4.leftPressed()) rs4.drawFrame(-1, Palette::Gray);
				else rs4.drawFrame(-1, Palette::Black);
				if (rs4.leftClicked()) ts[3].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(550, 230, Palette::Black);
				if (rr4.leftPressed()) rr4.drawFrame(-1, Palette::Gray);
				else rr4.drawFrame(-1, Palette::Black);
				if (rr4.leftClicked()) ts[3].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(550, 270, Palette::Black);
			}
			//5
			{
				FontAsset(U"SmartFont15")(U"No.5").drawAt(660, 83, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[4]]).drawAt(700, 110, ts[4].reachedZero() ? Palette::Red : Palette::Black);
				FontAsset(U"SmartFont30")(ts[4].mmss()).drawAt(700, 150, Palette::Black);
				if (r5.leftPressed()) r5.drawFrame(-1, Palette::Gray);
				else r5.drawFrame(-1, Palette::Black);
				if (r5.leftClicked()) ts[4].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(700, 190, Palette::Black);
				if (rs5.leftPressed()) rs5.drawFrame(-1, Palette::Gray);
				else rs5.drawFrame(-1, Palette::Black);
				if (rs5.leftClicked()) ts[4].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(700, 230, Palette::Black);
				if (rr5.leftPressed()) rr5.drawFrame(-1, Palette::Gray);
				else rr5.drawFrame(-1, Palette::Black);
				if (rr5.leftClicked()) ts[4].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(700, 270, Palette::Black);
			}
			//6
			{
				FontAsset(U"SmartFont15")(U"No.6").drawAt(810, 83, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[5]]).drawAt(850, 110, ts[5].reachedZero() ? Palette::Red : Palette::Black);
				FontAsset(U"SmartFont30")(ts[5].mmss()).drawAt(850, 150, Palette::Black);
				if (r6.leftPressed()) r6.drawFrame(-1, Palette::Gray);
				else r6.drawFrame(-1, Palette::Black);
				if (r6.leftClicked()) ts[5].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(850, 190, Palette::Black);
				if (rs6.leftPressed()) rs6.drawFrame(-1, Palette::Gray);
				else rs6.drawFrame(-1, Palette::Black);
				if (rs6.leftClicked()) ts[5].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(850, 230, Palette::Black);
				if (rr6.leftPressed()) rr6.drawFrame(-1, Palette::Gray);
				else rr6.drawFrame(-1, Palette::Black);
				if (rr6.leftClicked()) ts[5].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(850, 270, Palette::Black);
			}
			//7 ~ 12
			for (int i = 6; i < 12; i++) {
				FontAsset(U"SmartFont15")(U"No.", i + 1).drawAt(60 + (150 * (i - 6)), 333, Palette::Black);
				FontAsset(U"SmartFont30")(StatusS[statuses[i]]).drawAt(100 + (150 * (i - 6)), 360, Palette::Black);
				FontAsset(U"SmartFont30")(ts[i].mmss()).drawAt(100 + (150 * (i - 6)), 400, ts[i].reachedZero() ? Palette::Red : Palette::Black);
				if (r[i].leftPressed()) r[i].drawFrame(-1, Palette::Gray);
				else r[i].drawFrame(-1, Palette::Black);
				if (r[i].leftClicked()) ts[i].start();
				FontAsset(U"SmartFont30")(U"Start").drawAt(100 + (150 * (i - 6)), 440, Palette::Black);
				if (rs[i].leftPressed()) rs[i].drawFrame(-1, Palette::Gray);
				else rs[i].drawFrame(-1, Palette::Black);
				if (rs[i].leftClicked()) ts[i].pause();
				FontAsset(U"SmartFont30")(U"Stop").drawAt(100 + (150 * (i - 6)), 480, Palette::Black);
				if (rr[i].leftPressed()) rr[i].drawFrame(-1, Palette::Gray);
				else rr[i].drawFrame(-1, Palette::Black);
				if (rr[i].leftClicked()) ts[i].reset(limit_s);
				FontAsset(U"SmartFont30")(U"Reset").drawAt(100 + (150 * (i - 6)), 520, Palette::Black);
			}
			//All
			if (allr.leftPressed()) allr.drawFrame(-1, Palette::Gray);
			else allr.drawFrame(-1, Palette::Black);
			if (allr.leftClicked()) {
				for (int i = 0; i < 12;i++) {
					ts[i].start();
				}
			}
			FontAsset(U"SmartFont30")(U"AllStart").drawAt(100, 580, Palette::Black);
			if (allrs.leftPressed()) allrs.drawFrame(-1, Palette::Gray);
			else allrs.drawFrame(-1, Palette::Black);
			if (allrs.leftClicked()) {
				for (int i = 0; i < 12; i++) {
					ts[i].pause();
				}
			}
			FontAsset(U"SmartFont30")(U"AllStop").drawAt(250, 580, Palette::Black);
			if (allrr.leftPressed()) allrr.drawFrame(-1, Palette::Gray);
			else allrr.drawFrame(-1, Palette::Black);
			if (allrr.leftClicked()) {
				for (int i = 0; i < 12; i++) {
					ts[i].reset();
				}
			}
			FontAsset(U"SmartFont30")(U"AllReset").drawAt(400, 580, Palette::Black);
			//todo limit time up and down procotol;;
			if (upbtn.leftPressed()) upbtn.drawFrame(-1, Palette::Gray);
			else upbtn.drawFrame(-1, Palette::Black);
			if (upbtn.leftClicked()) {
				if (limit_s < 1000s) {
					limit_s += 30s;
					limit_forsend = Format(Parse<int>(limit_forsend) + 30);
					for (int i = 0; i < 12; i++) {
						ts[i].add(30s);
					}
				}
			}
			FontAsset(U"SmartFont30")(U"↑").drawAt(600, 580, Palette::Black);
			if (dwnbtn.leftPressed()) dwnbtn.drawFrame(-1, Palette::Gray);
			else dwnbtn.drawFrame(-1, Palette::Black);
			if (dwnbtn.leftClicked()) {
				if (limit_s > 30s) {
					limit_s -= 30s;
					limit_forsend = Format(Parse<int>(limit_forsend) - 30);
					for (int i = 0; i < 12; i++) {
						ts[i].add(-30s);
					}
				}
			}
			FontAsset(U"SmartFont30")(U"↓").drawAt(750, 580, Palette::Black);
			String limitminute = Format(Parse<int>(limit_forsend) % 60);
			if (limitminute.length() <= 1) limitminute.push_front('0');
			FontAsset(U"SmartFont30")((Parse<int>(limit_forsend) / 60), U":", limitminute).drawAt(680, 580, Palette::Black);
			FontAsset(U"SmartFont15")(U"来場人数").draw(1010, 75, Palette::Black);
			FontAsset(U"SmartFont30")(Format(ranjyoninzu, U"人")).draw(1010, 100, Palette::Black);
			FontAsset(U"SmartFont15")(U"ゲーム人気ランキング").draw(910, 135, Palette::Black);
			gamebetustr.clear();
			for (const auto& [key, value] : gamebetu) {
				gamebetustr += (key + U":" + Format(value) + U"\n");
			}
			FontAsset(U"SmartFont15")(gamebetustr).draw(910, 155, Palette::Black);
		}
	}
}