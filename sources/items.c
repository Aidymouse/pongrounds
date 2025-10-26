
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
		.description = "Makes your opponents paddle shorter", // Chastity Cage
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
		.description = "Gives you a sword", // Ceremonial Sword
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Time Wizards Chronometer",
		.description = "Slows down the ball in a zone", // Time Wizards Chronometer
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Bachelor of Psychology (Hons)",
		.description = "Lets you move vertically", // Bachelor of psychology (hons)
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Niekro Trading Card",
		.description = "Chance to launch a knuckeball on bounce", // Niekro Trading Card
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Nuclear Launch Codes",
		.description = "Lets you shoot a missile", // Nuclear Launch Codes
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Russian Secrets",
		.description = "Spikes the ball if it hits dead center of the paddle", // Russian Secrets
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Hydraulic Press",
		.description = "Chance to crush the ball", // Hydraulic Press
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Antique Game Console",
		.description = "Lets you reflect the balls movement", // Antique game console
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Cloning Vat",
		.description = "Lets you create another paddle", // Cloning Vat
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Legally Distinct Golden Snotch",
		.description = "Chance of doubling ball damage on paddle hit", // Legally Distinct Golden Snotch
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Yo Mama",
		.description = "Gives your paddle gravity", // Yo Momma
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Broken Mind Control Machine",
		.description = "Switch sides with opponent", // Broken Mind Control Machine
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
		.description = "Ball may run away from you", // Mutant Mouse
		.label_offset = 0,
		.description_offset = 0,
	},
	{
		.label = "Sentient Hand",
		.description = "Steal a random item from opponent at the start of the round", // Sentient Hand
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
