use stylist::yew::styled_component;
use yew::prelude::*;


#[derive(Properties, Clone, PartialEq)]
pub struct Props {
    pub soak: i32,
    pub size: i32,
    pub encumbrance: i32,
    #[prop_or_default]
    pub class: Option<String>,
}

#[styled_component(SoakSizeDisplay)]
pub fn soak_size_display(props: &Props) -> Html {
    let soak_display_str = if props.soak > 0 {
        format!("+{}", props.soak)
    } else {
        format!("{}", props.soak)
    };
    let size_display_str = if props.size > 0 {
        format!("+{}", props.size)
    } else {
        format!("{}", props.size)
    };
    let encumbrance_display_str = format!("{}", props.encumbrance);
    html! {
        <table class={&props.class}>
            <tr>
                <td><p>{"Size: "}</p></td>
                <td><p>{size_display_str}</p></td>
            </tr>
                <tr>
                <td><p>{"Soak: "}</p></td>
                <td><p>{soak_display_str}</p></td>
            </tr>
            <tr>
                <td><p>{"Encumbrance: "}</p></td>
                <td><p>{encumbrance_display_str}</p></td>
            </tr>
        </table>
    }
}