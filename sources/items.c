
#include "items.h"
#include "structs.h"

const ItemData item_info[] = {
	{
		.label = "Expired Panadol",
		.description = "Prevents you from\nlosing, once", // Expired Panadol
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Cherry Blossom Cloak",
		.description = "Lets you dash", // Cherry Blossom Cloak
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Hyper Gonadism",
		.description = "Makes your paddle\nwider",
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Chastity Cage",
		.description = "Shortens the\nopponents paddle", // Chastity Cage
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Nerd Glasses",
		.description = "Zooms out", // Nerd Glasses
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Ceremonial Sword",
		.description = "Gives the paddle\na sword", // Ceremonial Sword
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Time Wizards\nChronometer",
		.description = "Slows time near\nthe paddle", // Time Wizards Chronometer
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Bachelor of\nPsychology (Hons)",
		.description = "Enables up and down\nmovement", // Bachelor of psychology (hons)
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Niekro Trading Card",
		.description = "Each hit has a\nchance to launch\na knuckeball", // Niekro Trading Card
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Nuclear Launch\nCodes",
		.description = "Gives the paddle\na missile", // Nuclear Launch Codes
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Russian Secrets",
		.description = "Hitting the paddle\ndead-center spikes\nthe ball", // Russian Secrets
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Hydraulic Press",
		.description = "Each hit has a chance\nto crush the ball", // Hydraulic Press
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Antique Game\nConsole",
		.description = "Reflects the balls\nhorizontal movement\nwhen activated", // Antique game console
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Cloning Vat",
		.description = "Lets the paddle\nspawn a clone", // Cloning Vat
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Legally Distinct\nGolden Snotch",
		.description = "Each hit has a\nchance of doubling\nthe balls damage.", // Legally Distinct Golden Snotch
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Yo Mama",
		.description = "Gives the paddle\ngravity", // Yo Momma
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Broken Mind\nControl Machine",
		.description = "Switches sides with\nthe opponent when\nactivated", // Broken Mind Control Machine
		.label_offset = 0,
		.description_offset = 0,
	},
	/* 
	{
		.label = //"Underwear (Used)",
		.description = //"", // Underwear (Used)
		.label_offset = 0,
		.description_offset = 0,
	}
	*/
	{
		.label = "Mutant Mouse",
		.description = "Each hit has a\nchance of making the\nball run away", // Mutant Mouse
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Sentient Hand",
		.description = "Steal a random item\nfrom opponent at the\nstart of the round", // Sentient Hand
		.label_offset = 0,
		.description_offset = 0,
	},
};

const char *item_filenames[] = {
	"cards/panadol.png", // Expired Panadol
	"cards/cloak.png", // Cherry Blossom Cloak
	"cards/gonads.png", // Hypergonadism
	"cards/cage.png", // Chastity Cage
	"cards/glasses.png", // Nerd Glasses
	"cards/sword.png", // Ceremonial Sword
	"cards/time.png", // Time Wizards Chronometer
	"cards/bachelor of psychology.png", // Bachelor of psychology (hons)
	"cards/niekro.png", // Niekro Trading Card
	"cards/missile.png", // Nuclear Launch Codes
	"cards/russian secrets.png", // Russian Secrets
	"cards/hydraulic press.png", // Hydraulic Press 
	"cards/antique game console.png", // Antique Game Console 
	"cards/cloning vat.png", // Cloning Vat 
	"cards/legally distinct golden snotch.png", // Legally Distinct Golden Snotch 
	"cards/yo mama.png", // Yo Momma
	"cards/broken mind control machine.png", // Broken Mind Control Machine
	//"cards/todo.png", // Underwear (Used)
	"cards/mutant mouse.png", // Mutant Mouse
	"cards/sentient hand.png", // Sentient Hand
};

const char *small_item_filenames[] = {
	"small/panadol.png", // Expired Panadol
	"small/cloak.png", // Cherry Blossom Cloak
	"small/gonads.png", // Hypergonadism
	"small/cage.png", // Chastity Cage
	"small/glasses.png", // Nerd Glasses
	"small/sword.png", // Ceremonial Sword
	"small/time.png", // Time Wizards Chronometer
	"small/bachelor.png", // Bachelor of psychology (hons)
	"small/niekro.png", // Niekro Trading Card
	"small/missile.png", // Nuclear Launch Codes
	"small/russian secrets.png", // Russian Secrets
	"small/press.png", // Hydraulic Press 
	"small/antique game console.png", // Antique Game Console 
	"small/vat.png", // Cloning Vat 
	"small/snotch.png", // Snotch 
	"small/yo mama.png", // Yo Momma
	"small/mind control.png", // Broken Mind Control Machine
	//"small/todo.png", // Underwear (Used)
	"small/mutant mouse.png", // Mutant Mouse
	"small/hand.png", // Sentient Hand
};
