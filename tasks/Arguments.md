`class:meta`
- tags: #☑️Task
- title: Arguments
- wikilinks: 
- parent: [[TUI]]
- created: 2023-05-16T22:00:44+03:00
- completed: date

### Description

Create new arguments engine. Implement argument TUI

Example constructor input:
```C++
std::vector<Terminal::ArgumentList> arguments = 
{
{
    .id=Terminal::id_type(0),
    .name="Argument list 1",
    .validator=fn1, // bool validate_argument_list(std::vector<std::pair<std::string, std::string>> items)
    {
        {
        .name="Argument 1",
        .short='1',
        .long="argument-1",
        .has_value=true,
        .validator=fn2, // bool validate_argument(std::pair<std::string, std::string> value)
        },
        .description="Description for argument 1",
        .default_value=fn3, std::string get_default()
    },
    {
        {
        .name="Argument 2",
        .short='2',
        .long="argument-2",
        .has_value=true,
        .validator=, // use std::optional
        },
        .description="Description for argument 2",
        .default_value= // use std::optional
    }
}
}
```
