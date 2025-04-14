#let template(cfg: none, body) = {
    assert(cfg != none)
    assert(cfg.project != none)
    assert(cfg.project.code != none)
    assert(cfg.project.name != none)
    assert(cfg.student != none)
    assert(cfg.student.name != none)
    assert(cfg.student.group != none)
    assert(cfg.agreed_by.name != none)
    assert(cfg.agreed_by.position != none)
    assert(cfg.approved_by.name != none)
    assert(cfg.approved_by.position != none)
    assert(cfg.university_name != none)
    assert(cfg.faculty_name != none)
    assert(cfg.edu_program_name != none)
    assert(cfg.year != none)

    let un(n) = "_" * n

    let approval_page_code = cfg.project.code + "-ЛУ"

    // отметка об учёте и хранении по ГОСТ 19.601-78
    let storage_table = {
        set text(size: 10pt)
        place(
            bottom + left,
            dx: 5mm,
            dy: -10mm,
            rotate(
                -90deg,
                reflow: true,
                table(
                    columns: (25mm, 35mm, 25mm, 25mm, 35mm),
                    rows: (5mm, 7mm),
                    align: center,
                    [Инв.№ подп], [Подп. и дата], [Взам. инв.№], [Инв.№ дубл.], [Подп. и дата]
                )
            )
        )
    }

    // Лист утверждения
    let approval_page = {
        let top_banner = [
            #set par(spacing: 0.65em)

            #text(weight: "bold", cfg.university_name)

            #cfg.faculty_name

            #cfg.edu_program_name
        ]

        let approve_table = grid(
            columns: (1fr, 1fr),
            inset: (
                x: 10mm,
                y: 3mm,
            ),
            align: center,

            [
                СОГЛАСОВАНО

                #cfg.agreed_by.position
            ],
            [
                УТВЕРЖДЕНО

                #cfg.approved_by.position
            ],

            box[
                #un(13) #cfg.agreed_by.name

                "#un(3)" #un(13) #cfg.year г.
            ],
            [
                #un(13) #cfg.approved_by.name

                "#un(3)" #un(13) #cfg.year г.
            ]
        )

        let center_banner = [
            #set text(size: 14pt, weight: "bold")
            #set par(spacing: 2em)

            #par(spacing: 0.65em, cfg.project.name)

            Программа и методика испытаний

            ЛИСТ УТВЕРЖДЕНИЯ

            #approval_page_code
        ]

        let student_info = align(right)[
            #set par(spacing: 1em)

            Исполнитель:

            Студент группы #cfg.student.group

            #un(13) / #cfg.student.name /

            "#un(3)" #un(15) #cfg.year г.
        ]

        let bottom_banner = [
            #set text(weight: "bold")

            #cfg.year
        ]

        page(
            header: none,
            footer: none,
            margin: (
                left: 20mm,
                right: 10mm,
                top: 25mm,
                bottom: 15mm,
            ),
            foreground: storage_table
        )[
            #set align(center)

            #grid(
                columns: (1fr),
                row-gutter: 1fr,
                top_banner,
                approve_table,
                center_banner,
                student_info,
                bottom_banner,
            )
        ]

        counter(page).update(1)
    }

    let title_page = {
        let top_banner = [
            #set align(left)
            #set par(spacing: 2em)

            УТВЕРЖДЕН

            #approval_page_code
        ]

        let center_banner = [
            #set text(size: 14pt, weight: "bold")
            #set par(spacing: 2em)

            #par(spacing: 0.65em, cfg.project.name)

            Программа и методика испытаний

            #cfg.project.code

            Листов #context {counter(page).final().at(0)}
        ]

        let bottom_banner = [
            #set text(weight: "bold")

            #cfg.year
        ]

        page(
            header: none,
            footer: none,
            margin: (
                left: 20mm,
                right: 10mm,
                top: 25mm,
                bottom: 15mm,
            ),
            foreground: storage_table
        )[
            #set align(center)

            #grid(
                columns: (1fr),
                row-gutter: 1fr,
                top_banner,
                center_banner,
                bottom_banner,
            )
        ]
    }

    let list_registraion_changes = {

        let list_name = [
            #set text(size: 14pt, weight: "bold")
            #set par(spacing: 2em)
            ЛИСТ РЕГИСТРАЦИИ ИЗМЕНЕНИЙ
        ]

        let changes_table = figure(
            table(
                columns: (10mm,) + (15mm, ) * 4 + (20mm, ) * 2 + (auto, ) + (auto, ) * 2,
                
                rows: (auto, ) * 3 + (9.5mm, ) * 21,

                table.cell(
                colspan: 10,
                align: horizon,
                [Лист регистрации изменений]
                ), 
                table.cell(
                colspan: 5,
                align: horizon,
                [Номера листов (страниц)]
                ),

                table.cell(
                rowspan: 2,
                align: horizon,
                [Всего листов (страниц в докум.)],
                ),
                table.cell(
                rowspan: 2,
                align: horizon,
                [№ документа],
                ),
                table.cell(
                rowspan: 2,
                align: horizon,
                [Входящий № сопроводительного докум. и дата],
                ),
                table.cell(
                rowspan: 2,
                align: horizon,
                [Подп.],
                ),
                table.cell(
                rowspan: 2,
                align: horizon,
                [Дата],
                ),

                table.cell(
                align: horizon,
                rotate(-90deg, reflow: true)[
                    Изм.
                ],
                ),
                table.cell(
                align: horizon,
                rotate(-90deg, reflow: true)[
                    Измененных
                ],
                ),
                table.cell(
                align: horizon,
                rotate(-90deg, reflow: true)[
                    Замененных
                ],
                ),
                table.cell(
                align: horizon,
                rotate(-90deg, reflow: true)[
                    Новых
                ],
                ),
                table.cell(
                align: horizon,
                rotate(-90deg, reflow: true)[
                    Аннулированных
                ],
                ),
            )
        )

        page(
            header: none,
            footer: none,
            margin: (
                left: 20mm,
                right: 10mm,
                top: 25mm,
                bottom: 15mm,
            ),
        )[
            #set align(center)

            #grid(
                columns: (1fr),
                row-gutter: 1fr,
                list_name, 
                changes_table,
            )
        ]
    }

    let outline = {
        pagebreak(weak: true)

        {
            set align(center)
            set text(weight: "bold")

            [СОДЕРЖАНИЕ]
        }

        outline(
            title: none,
            indent: 5mm,
        )
    }

    let outline_and_normal_pages = {
        set page(
            margin: (
                top: 25mm,
                left: 20mm,
                right: 10mm,
                bottom: 47mm,
            ),
            header: [
                #set align(center)
                #set text(weight: "bold")

                #context counter(page).display()

                #cfg.project.code
            ],
            footer: [
                #table(
                    columns: (2fr, 1fr, 1fr, 1fr, 1fr),
                    align: center,
                    rows: 7mm,

                    [],           [],             [],             [],             [],
                    [Изм.],           [Лист],         [№ докум.],     [Подп.],        [Дата],
                    cfg.project.code, [],             [],             [],             [],
                    [Инв. № подл.],   [Подп. и дата], [Взам. Инв. №], [Инв. № дубл.], [Подп. и дата],
                )
            ]
        )

        set par(
            //first-line-indent: 1em,
            justify: true,
            leading: 1em,
        )

        set heading(numbering: "1.")

        show heading.where(level: 1): h => {
            set align(center)
            set text(weight: "bold", size: 12pt)

            pagebreak(weak: true)
            if h.numbering != none [
                #counter(heading).display(h.numbering) #h.body
            ] else [
                #h.body
            ]

        }

        show heading.where(level: 2): h => {
            set text(weight: "bold", size: 12pt)

            [#counter(heading).display() #h.body]
        }

        pagebreak(weak: true)
        align(center, text(weight: "bold", size: 12pt, [АННОТАЦИЯ]))
        cfg.annotation

        outline
        body
    }

    set text(
        lang: "ru",
        size: 12pt,
        font: "Times New Roman"
    )

    approval_page
    title_page
    outline_and_normal_pages
    list_registraion_changes
}
