

void main() {
    dsll::Language keyboardLang();
    keyboardLang.addExtension(kybdInterface);
    keyboardLang.loadLanguage(".../keyboard.dsl");
    dsll::Instance keyboard = keyboardLang.parse(".../main_layout")
    keyboard.execute("main");
    try {
        var baseLayer = keyboard.getObjectHandle("baseLayer");
    } catch (UnknownHandle &e) {}

    createLayer(baseLayer.get<int>("Type"), baseLayer.get<dsll::Function>("effect"));
}