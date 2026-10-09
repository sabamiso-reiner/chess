#include <stdio.h>
#include <ctype.h>	//tolower使うのに必要
#include <stdlib.h> //malloc,free使うのに必要
#include <string.h> //strcmp使うのに必要

#ifdef _WIN32
#include <windows.h> //コンソールの文字コード設定に必要
#endif

#define true 1
#define false 0

enum color
{
	InvalidColor,
	White,
	Black
}; // 駒の色

enum exist
{
	Nonexist,
	exist
}; // 駒が存在しているか

enum piece
{
	InvalidPiece,
	Pawn,
	Knight,
	Bishop,
	Rook,
	Queen,
	King
}; // 駒の種類

enum moveLog
{
	moved,
	Nonmove
}; // 駒が一回以上動いているか

typedef struct
{
	enum color pieceColor;	   // 駒の色、駒をおいてはいけない：０　白：１　黒：２
	enum exist pieceExist;	   // 駒が存在するか、存在しない：０　存在する：１
	enum piece kind;		   // 駒の種類
	enum moveLog pieceMoveLog; // 一回以上動いたか
} Piece;

int MoveWPawn(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{ // 白のポーンの動き方があっているか調べる
	if (((sRank + 1) == dRank) && (sFile == dFile) && (gameboard[dRank][dFile].pieceExist == Nonexist))
	{ // 前が空いていれば１ランク前進
		return true;
	}
	else if (((sRank + 2) == dRank) && (sFile == dFile) && (gameboard[sRank][sFile].pieceMoveLog == Nonmove) && (gameboard[sRank + 1][sFile].pieceExist == Nonexist) && (gameboard[dRank][dFile].pieceExist == Nonexist))
	{ // 始めての時だけ、間と行き先が空いていれば２ランク前進可能
		return true;
	}
	else if (((sRank + 1) == dRank) && ((sFile - 1) == dFile) && (gameboard[dRank][dFile].pieceColor == Black))
	{ // 敵駒がいる左斜め前へ進む
		return true;
	}
	else if (((sRank + 1) == dRank) && ((sFile + 1) == dFile) && (gameboard[dRank][dFile].pieceColor == Black))
	{ // 敵駒がいる右斜め前へ進む
		return true;
	}
	else
	{ // ダメな動き
		return false;
	}
}

int MoveBPawn(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{ // 黒のポーンの動き方があっているか調べる
	if (((sRank - 1) == dRank) && (sFile == dFile) && (gameboard[dRank][dFile].pieceExist == Nonexist))
	{ // 前が空いていれば１ランク前進
		return true;
	}
	else if (((sRank - 2) == dRank) && (sFile == dFile) && (gameboard[sRank][sFile].pieceMoveLog == Nonmove) && (gameboard[sRank - 1][sFile].pieceExist == Nonexist) && (gameboard[dRank][dFile].pieceExist == Nonexist))
	{ // 始めての時だけ、間と行き先が空いていれば２ランク前進可能
		return true;
	}
	else if (((sRank - 1) == dRank) && ((sFile - 1) == dFile) && (gameboard[dRank][dFile].pieceColor == White))
	{ // 敵駒がいる右斜め前へ進む
		return true;
	}
	else if (((sRank - 1) == dRank) && ((sFile + 1) == dFile) && (gameboard[dRank][dFile].pieceColor == White))
	{ // 敵駒がいる左斜め前へ進む
		return true;
	}
	else
	{ // ダメな動き
		return false;
	}
}

int MoveKnight(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{ // ナイトの動きがあっているか調べる
	if (((sRank + 2) == dRank) && (((sFile + 1) == dFile) || ((sFile - 1) == dFile)))
	{ // ２ランク前進して右左
		return true;
	}
	else if (((sRank - 2) == dRank) && (((sFile + 1) == dFile) || ((sFile - 1) == dFile)))
	{ // ２ランク後退して右左
		return true;
	}
	else if (((sFile - 2) == dFile) && (((sRank + 1) == dRank) || ((sRank - 1) == dRank)))
	{ // ２ファイル左に行って上下
		return true;
	}
	else if (((sFile + 2) == dFile) && (((sRank + 1) == dRank) || ((sRank - 1) == dRank)))
	{ // ２ファイル右に行って上下
		return true;
	}
	else
	{
		return false; // ダメな動き
	}
}

int MoveBishop(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{ // ビショップの動きがあっているか調べる
	int f = sFile;
	int r = sRank;

	int x = (sFile < dFile) ? 1 : -1; // 右(1)に移動するか左(-1)に移動するか、今ビショップのいる位置を基準とした、行先の象限の右左
	int y = (sRank < dRank) ? 1 : -1; // 上(1)に移動するか下(-1)に移動するか、今ビショップのいる位置を基準とした、行先の象限の上下

	if ((x == 1) && (y == 1))
	{ // 第１象限へ
		for (f += 1, r += 1; (f < dFile) && (r < dRank) && (gameboard[r][f].pieceExist == Nonexist); f++, r++)
		{ // 行きたいところの一歩前までに邪魔な駒がないかチェック
		}
	}
	else if ((x == -1) && (y == 1))
	{ // 第２象限へ
		for (f -= 1, r += 1; (f > dFile) && (r < dRank) && (gameboard[r][f].pieceExist == Nonexist); f--, r++)
		{ // 行きたいところの一歩前までに邪魔な駒がないかチェック
		}
	}
	else if ((x == -1) && (y == -1))
	{ // 第３象限へ
		for (f -= 1, r -= 1; (f > dFile) && (r > dRank) && (gameboard[r][f].pieceExist == Nonexist); f--, r--)
		{ // 行きたいところの一歩前までに邪魔な駒がないかチェック
		}
	}
	else if ((x == 1) && (y == -1))
	{ // 第４象限へ
		for (f += 1, r -= 1; (f < dFile) && (r > dRank) && (gameboard[r][f].pieceExist == Nonexist); f++, r--)
		{ // 行きたいところの一歩前までに邪魔な駒がないかチェック
		}
	}

	if ((f == dFile) && (r == dRank))
	{ // 行きたいところに何も問題がない
		return true;
	}
	else
	{				  // 何かしらの問題があった、邪魔な駒がいるとfalseに分岐
		return false; 
	}
}

int MoveRook(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{ // ルークの動き方をチェック
	int f = sFile;
	int r = sRank;
	if (sFile < dFile)
	{ // 右側に行く
		for (f += 1; (f < dFile) && (gameboard[r][f].pieceExist == Nonexist); f++)
		{
		}
	}
	else if (sFile > dFile)
	{ // 左側へ行く
		for (f -= 1; (f > dFile) && (gameboard[r][f].pieceExist == Nonexist); f--)
		{
		}
	}
	else if (sRank < dRank)
	{ // 黒の方へ行く
		for (r += 1; (r < dRank) && (gameboard[r][f].pieceExist == Nonexist); r++)
		{
		}
	}
	else if (sRank > dRank)
	{ // 白の方へ行く
		for (r -= 1; (r > dRank) && (gameboard[r][f].pieceExist == Nonexist); r--)
		{
		}
	}

	if ((f == dFile) && (r == dRank))
	{ // 行きたいところに何も問題がない
		return true;
	}
	else
	{ // 何かしらの問題があった、邪魔な駒がいるとfalseに分岐
		return false;
	}
}

int MoveQueen(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{
	if ((sFile == dFile) || (sRank == dRank))
	{ // ルーク型の動きにしかならない
		return MoveRook(gameboard, sFile, dFile, sRank, dRank);
	}
	else
	{ // ビショップ型の動きにしかならない
		return MoveBishop(gameboard, sFile, dFile, sRank, dRank);
	}
}

int MoveKing(Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{
	if ((sRank + 1 == dRank) && ((sFile - 1 == dFile) || (sFile == dFile) || (sFile + 1 == dFile)))
	{ // １ランク前進して真ん中、右、左
		return true;
	}
	else if ((sRank == dRank) && ((sFile - 1 == dFile) || (sFile + 1 == dFile)))
	{ // 右、左
		return true;
	}
	else if ((sRank - 1 == dRank) && ((sFile - 1 == dFile) || (sFile == dFile) || (sFile + 1 == dFile)))
	{ // １ランク後退して真ん中、右、左
		return true;
	}
	else
	{
		return false;
	}
}

int Move(
	Piece gameboard[9][9], int sFile, int dFile, int sRank, int dRank)
{ // 駒の動き方があっているか調べる
	switch (gameboard[sRank][sFile].kind)
	{
	case Pawn:
		if (gameboard[sRank][sFile].pieceColor == White)
		{
			return MoveWPawn(gameboard, sFile, dFile, sRank, dRank);
		}
		else
		{
			return MoveBPawn(gameboard, sFile, dFile, sRank, dRank);
		}
		break;
	case Knight:
		return MoveKnight(gameboard, sFile, dFile, sRank, dRank);
		break;
	case Bishop:
		return MoveBishop(gameboard, sFile, dFile, sRank, dRank);
		break;
	case Rook:
		return MoveRook(gameboard, sFile, dFile, sRank, dRank);
		break;
	case Queen:
		return MoveQueen(gameboard, sFile, dFile, sRank, dRank);
		break;
	case King:
		return MoveKing(gameboard, sFile, dFile, sRank, dRank);
		break;
	}
}

int convertFile(char file)
{
	// ファイル（列）の文字を数値に変換 ('a' -> 1, ..., 'h' -> 8)
	if (file >= 'a' && file <= 'h')
	{
		return file - 'a' + 1;
	}
	return 0; // 無効な場合は0を返す
}

char convertFileToChar(int file)
{
	// 数値をチェスのファイル（列）に対応する文字に変換 (1 -> 'a', ..., 8 -> 'h')
	if (file >= 1 && file <= 8)
	{
		return 'a' + (file - 1);
	}
	return '\0'; // 無効な場合はNULL文字を返す
}

int convertRank(char rank)
{
	// ランク（行）の文字を数値に変換 ('1' -> 1, ..., '8' -> 8)
	if (rank >= '1' && rank <= '8')
	{
		return rank - '1' + 1;
	}
	return 0; // 無効な場合は0を返す
}

int *decideStart(Piece gameboard[9][9], enum color color)
{
	int *start = (int *)malloc(2 * sizeof(int)); // メモリを動的に確保  malloc使ってる
	char input[3];								 // ファイルとランクの入力用 (例: "e2")]
	char file;									 // 小文字に変換して扱いやすくする
	char rank;									 // 2文字目はそのまま使用
	int sFile;									 // ファイルを数値に変換
	int sRank;									 // ランクを数値に変換
	while (1)
	{ // 動かす駒を確定
		printf("移動したい駒のファイルとランクを入力してください（例: e2）：");
		scanf("%2s", input);	   // 2文字の入力を受け付ける
		file = tolower(input[0]);  // 小文字に変換して扱いやすくする
		rank = input[1];		   // 2文字目はそのまま使用
		sFile = convertFile(file); // ファイルを数値に変換
		sRank = convertRank(rank); // ランクを数値に変換
		if (sFile == 0 || sRank == 0)
		{ // 盤面外を指定した時
			printf("指定した場所は盤面外です\n");
		}
		else if (gameboard[sRank][sFile].pieceColor != color)
		{ // colorの駒以外を指定した時
			printf("指定した駒は動かせないか、駒が存在しません\n");
		}
		else
		{				  // 正しい駒を指定した時
			printf("\n"); // 改行して質問を見やすくする
			break;
		}
		printf("\n"); // 改行して質問を見やすくする
	}
	start[0] = sFile;
	start[1] = sRank;
	return start;
}

int *decideDestination(Piece gameboard[9][9], enum color color, int sFile, int sRank)
{
	int *destination = (int *)malloc(2 * sizeof(int)); // メモリを動的に確保 後で確認
	char input[3];									   // ファイルとランクの入力用 (例: "e2")]
	char file;										   // 小文字に変換して扱いやすくする
	char rank;										   // 2文字目はそのまま使用
	int dFile;										   // ファイルを数値に変換
	int dRank;										   // ランクを数値に変換
	while (1)
	{ // 動かす場所を確定
		printf("動かす駒の選択をやり直す場合は例のように指定してください（例： z9）\n");
		printf("移動先のファイルとランクを入力してください（例: e4）: ");
		scanf("%2s", input);	   // 2文字の入力を受け付ける
		file = tolower(input[0]);  // 小文字に変換して扱いやすくする
		rank = input[1];		   // 2文字目はそのまま使用
		dFile = convertFile(file); // ファイルを数値に変換
		dRank = convertRank(rank); // ランクを数値に変換
		if ((dFile == 0) && (dRank == 0))
		{
			printf("\n"); // 改行して質問を見やすくする
			printf("駒の選択に戻ります\n");
			break;
		}
		else if (dFile == 0 || dRank == 0 || (gameboard[dRank][dFile].pieceColor == color) || !(Move(gameboard, sFile, dFile, sRank, dRank)))
		{ // 盤面外を指定した、または指定した先に白駒がいる時
			printf("指定した場所には動かせません\n");
		}
		else
		{				  // 正しい場所を指定した時
			printf("\n"); // 改行して質問を見やすくする
			break;
		}
		printf("\n"); // 改行して質問を見やすくする
	}
	destination[0] = dFile;
	destination[1] = dRank;
	return destination;
}

void Promotion(Piece gameboard[9][9], enum color color, int File, int Rank)
{ // プロモーションの実行
	int promotion;
	do
	{
		printf("プロモーションを行います\n");
		printf("どの駒になりますか？\n");
		printf("クイーン：1　ルーク：2　ビショップ：3　ナイト：4（半角数字）：");
		if (scanf("%d", &promotion) != 1)
		{ // 数字以外が入力された時は、その行を読み捨てて聞き直す
			int c;
			while ((c = getchar()) != '\n' && c != EOF)
			{
			}
			if (c == EOF)
			{ // 入力が終わっている時は聞き直しても無駄なのでクイーンにする
				promotion = 1;
				break;
			}
			promotion = 0;
		}
	} while (!((1 <= promotion) && (promotion <= 4)));
	switch (promotion)
	{
	case 1:
		gameboard[Rank][File].kind = Queen;
		break;
	case 2:
		gameboard[Rank][File].kind = Rook;
		break;
	case 3:
		gameboard[Rank][File].kind = Bishop;
		break;
	case 4:
		gameboard[Rank][File].kind = Knight;
		break;
	}
}

void jugdePromotion(Piece gameboard[9][9], enum color color)
{ // プロモーションできるか判定
	if (color == White)
	{ // 白のポーン
		for (int i = 1; i <= 8; i++)
		{
			if (gameboard[8][i].kind == Pawn)
			{
				Promotion(gameboard, White, i, 8);
			}
		}
	}
	else if (color == Black)
	{ // 黒のポーン
		for (int i = 1; i <= 8; i++)
		{
			if (gameboard[1][i].kind == Pawn)
			{
				Promotion(gameboard, Black, i, 1);
			}
		}
	}
	else
	{ // 白か黒以外を指定された時
	  // 何もしない
	}
}

int judgeEnd(Piece gameboard[9][9], enum color color) // キングがいなくなったらゲーム終了
{
	int king = 0; // 盤面上にいる指定された色のキングの数
	for (int i = 1; i <= 8; i++)
	{ // ランクを見ていく
		for (int j = 1; j <= 8; j++)
		{ // ファイルを見ていく
			if ((gameboard[i][j].pieceColor == color) && (gameboard[i][j].kind == King))
			{
				king = 1;
			}
		}
	}
	if (king == 1)
	{
		return false; // 指定された色のキングがいる
	}
	else
	{
		return true; // 指定された色のキングがいない
	}
}

void setFirstBoard(Piece gameboard[9][9])
{												 // 初期盤面の生成
	Piece Wpawn = {White, exist, Pawn, Nonmove}; // 白のポーン
	Piece Bpawn = {Black, exist, Pawn, Nonmove}; // 黒のポーン
	for (int i = 1; i <= 8; i++)
	{
		gameboard[2][i] = Wpawn;
		gameboard[7][i] = Bpawn;
	}
	Piece WKnight = {White, exist, Knight, Nonmove}; // 白のナイト
	Piece BKnight = {Black, exist, Knight, Nonmove}; // 黒のナイト
	gameboard[1][2] = WKnight;
	gameboard[1][7] = WKnight;
	gameboard[8][2] = BKnight;
	gameboard[8][7] = BKnight;
	Piece WBshop = {White, exist, Bishop, Nonmove}; // 白のビショップ
	Piece BBshop = {Black, exist, Bishop, Nonmove}; // 黒のビショップ
	gameboard[1][3] = WBshop;
	gameboard[1][6] = WBshop;
	gameboard[8][3] = BBshop;
	gameboard[8][6] = BBshop;
	Piece WRook = {White, exist, Rook, Nonmove}; // 白のルーク
	Piece BRook = {Black, exist, Rook, Nonmove}; // 黒のルーク
	gameboard[1][1] = WRook;
	gameboard[1][8] = WRook;
	gameboard[8][1] = BRook;
	gameboard[8][8] = BRook;
	Piece WQueen = {White, exist, Queen, Nonmove}; // 白のクイーン
	Piece BQueen = {Black, exist, Queen, Nonmove}; // 黒のクイーン
	gameboard[1][4] = WQueen;
	gameboard[8][4] = BQueen;
	Piece WKing = {White, exist, King, Nonmove}; // 白のキング
	Piece BKing = {Black, exist, King, Nonmove}; // 黒のキング
	gameboard[1][5] = WKing;
	gameboard[8][5] = BKing;
}

int asciiMode = false; // trueなら駒を英字で表示する（駒の記号を表示できない環境向け）

void printPieceAscii(Piece piece)
{ // 駒を表す英字を出力、白は大文字・黒は小文字
	const char *letters = " PNBRQK"; // enum pieceの順に対応
	if (piece.pieceColor == White)
	{
		printf(" %c ", letters[piece.kind]);
	}
	else if (piece.pieceColor == Black)
	{
		printf(" %c ", tolower(letters[piece.kind]));
	}
	else
	{
		printf(" . ");
	}
}

void printPiece(Piece piece)
{ // 駒を表す記号（絵）を出力
	if (asciiMode)
	{
		printPieceAscii(piece);
	}
	else if (piece.pieceColor == White)
	{ // 白色の駒
		switch (piece.kind)
		{ // 駒の種類
		case Pawn:
			printf(" ♟ ");
			break;
		case Knight:
			printf(" ♞ ");
			break;
		case Bishop:
			printf(" ♝ ");
			break;
		case Rook:
			printf(" ♜ ");
			break;
		case Queen:
			printf(" ♛ ");
			break;
		case King:
			printf(" ♚ ");
			break;
		}
	}
	else if (piece.pieceColor == Black)
	{ // 黒色の駒
		switch (piece.kind)
		{ // 駒の種類
		case Pawn:
			printf(" ♙ ");
			break;
		case Knight:
			printf(" ♘ ");
			break;
		case Bishop:
			printf(" ♗ ");
			break;
		case Rook:
			printf(" ♖ ");
			break;
		case Queen:
			printf(" ♕ ");
			break;
		case King:
			printf(" ♔ ");
			break;
		}
	}
	else
	{
		printf(" . ");
	}
}

void printGameBoard(Piece gameboard[9][9])
{ // 現在の盤面を出力
	printf("  +---+---+---+---+---+---+---+---+\n");
	for (int i = 8; i >= 1; i--)
	{
		printf("%d |", i);
		for (int j = 1; j <= 8; j++)
		{
			printPiece(gameboard[i][j]);
			printf("|");
		}
		printf("\n");
		printf("  +---+---+---+---+---+---+---+---+\n");
	}
	printf("    a   b   c   d   e   f   g   h");
	printf("\n");
}

#ifdef _WIN32
UINT originalOutputCP; // 終了時に戻すための元のコードページ
UINT originalInputCP;

void restoreConsoleCP(void)
{ // コンソールのコードページを実行前の状態に戻す
	SetConsoleOutputCP(originalOutputCP);
	SetConsoleCP(originalInputCP);
}

void waitBeforeClose(void)
{ // ダブルクリックで起動した時、結果を読む前にウィンドウが閉じないように待つ
	DWORD processList[2];
	if (GetConsoleProcessList(processList, 2) == 1)
	{ // コンソールを使っているのがこのプログラムだけ＝ダブルクリックで起動された
		int c;
		while ((c = getchar()) != '\n' && c != EOF)
		{ // 前の入力の残り（改行など）を読み捨てる
		}
		printf("Enterキーを押すと終了します\n");
		getchar();
	}
}
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
	// ソース中の文字列はUTF-8なので、コンソールもUTF-8で入出力させる
	originalOutputCP = GetConsoleOutputCP();
	originalInputCP = GetConsoleCP();
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	atexit(restoreConsoleCP);
	atexit(waitBeforeClose); // atexitは登録と逆順に実行されるので、コードページを戻す前に待つ
#endif
	for (int i = 1; i < argc; i++)
	{ // --ascii を指定すると駒を英字で表示する
		if (strcmp(argv[i], "--ascii") == 0)
		{
			asciiMode = true;
		}
	}
	printf("チェック・チェックメイトの判定は存在しません\n");
	printf("キャスリングは使えません\n");
	printf("アンパッサンは使えません\n");
	char Wplayer[60]; // 白側のプレイヤー名
	char Bplayer[60]; // 黒側のプレイヤー名
	printf("先手となる白側のプレイヤー名を教えてください：");
	scanf("%59s", Wplayer); // バッファの大きさを超えないように制限
	printf("後手となる黒側のプレイヤー名を教えてください：");
	scanf("%59s", Bplayer); // バッファの大きさを超えないように制限
	Piece gameboard[9][9] = {0}; // 配列を９行９列で生成し初期化するが、０行目と０列目は使わずに８行８列の盤面を作る。配列操作を見やすく・やりやすくするため。右へ行くほど列の値が増え、上へ行くほど行の値が増える。
	setFirstBoard(gameboard);	 // 初期盤面の生成
	printf("ゲームスタート！\n");
	printGameBoard(gameboard);
	int turn = 0;	  // ターン数
	char input[3];	  // ファイルとランクの入力用 (例: "e2")
	int *start;		  // 関数を使用して動かす駒の位置を受け取る
	int sFile;		  // ファイルを数値に変換、動かす駒のファイル
	int sRank;		  // ランクを数値に変換、動かす駒のランク
	int *destination; // 関数を使用して動かす先の位置を受け取る
	int dFile;		  // ファイルを数値に変換、動かす先のファイル
	int dRank;		  // ランクを数値に変換、動かす先のランク
	char sfile;		  // 動かす駒のファイル、スコアに書き出すために使う
	char dfile;		  // 動かす先のファイル、スコアに書き出すために使う
	FILE *fp;
	if ((fp = fopen("Score", "w")) == NULL)
	{
		printf("ファイルをオープンできません。\n");
	}
	else
	{ // ファイルをオープンできた
		// スコアに選手名を記録
		fprintf(fp, "白：%s\n", Wplayer); // スコアに選手名を記録
		fprintf(fp, "黒：%s\n", Bplayer); // スコアに選手名を記録
		fprintf(fp, "\n");				  // スコアを見やすいように改行
		while (1)
		{
			turn += 1;
			if ((turn % 2) == 1)
			{ // 奇数ターン・白の手番
				printf("%dターン目：", turn);
				printf("白の手番\n");
			ReWhite: // 動かす駒の再選択に使うラベル
				start = decideStart(gameboard, White);
				sFile = start[0];
				sRank = start[1];
				free(start); // メモリ解放 free使ってる
				destination = decideDestination(gameboard, White, sFile, sRank);
				dFile = destination[0];
				dRank = destination[1];
				free(destination); // メモリ解放 free使ってる
				if ((dFile == 0) && (dRank == 0))
				{ // 駒の選択に戻る
					goto ReWhite;
				}
			}
			else
			{ // 偶数ターン・黒の手番
				printf("%dターン目：", turn);
				printf("黒の手番\n");
			ReBlack: // 動かす駒の再選択に使うラベル
				start = decideStart(gameboard, Black);
				sFile = start[0];
				sRank = start[1];
				free(start); // メモリ解放 free使ってる
				destination = decideDestination(gameboard, Black, sFile, sRank);
				dFile = destination[0];
				dRank = destination[1];
				free(destination); // メモリ解放 free使ってる
				if ((dFile == 0) && (dRank == 0))
				{ // 駒の選択に戻る
					goto ReBlack;
				}
			}
			gameboard[dRank][dFile] = gameboard[sRank][sFile]; // 駒を移動
			gameboard[dRank][dFile].pieceMoveLog = moved;	   // 一回以上動いたように設定する
			Piece none = {InvalidColor, Nonexist, InvalidPiece, moved};
			gameboard[sRank][sFile] = none;	  // 移動元は空にする
			jugdePromotion(gameboard, White); // 白のプロモーションの判定
			jugdePromotion(gameboard, Black); // 黒のプロモーションの判定
			printGameBoard(gameboard);
			// スコアを書くための処理
			sfile = convertFileToChar(sFile); // ファイルをchar型へ
			dfile = convertFileToChar(dFile); // ファイルをchar型へ
			if ((turn % 2) == 1)
			{ // 白のターン
				fprintf(fp, "%dターン目：白の手番\n", turn);
				fprintf(fp, "%c%d→%c%d\n", sfile, sRank, dfile, dRank);
				fprintf(fp, "\n");
			}
			else
			{ // 黒のターン
				fprintf(fp, "%dターン目：黒の手番\n", turn);
				fprintf(fp, "%c%d→%c%d\n", sfile, sRank, dfile, dRank);
				fprintf(fp, "\n");
			}
			// キングが盤面上にいなくなった時の処理
			if (judgeEnd(gameboard, White) == true)
			{ // 白のキングがいない
				printf("ゲーム終了\n");
				printf("黒：%sの勝利\n", Bplayer);
				fprintf(fp, "黒：%sの勝利\n", Bplayer);
				break;
			}
			else if (judgeEnd(gameboard, Black) == true)
			{ // 黒のキングがいない
				printf("ゲーム終了\n");
				printf("白：%sの勝利\n", Wplayer);
				fprintf(fp, "白：%sの勝利\n", Wplayer);
				break;
			}
		}
		fclose(fp);
	}
	return 0;
}
