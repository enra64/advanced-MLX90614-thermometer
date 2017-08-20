/// ClickListener defines a function that returns void, and gets a boolean as its only parameter. If it is true,
/// the button is activated. This means the input value at its pin is low!
typedef void (*ClickListener)(bool);