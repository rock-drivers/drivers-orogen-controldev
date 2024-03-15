# frozen_string_literal: true

using_task_library "controldev"

describe OroGen.controldev.RawCommandAggregator do
    attr_reader :task
    before do
        @task_m = OroGen.controldev.RawCommandAggregator
                        .deployed_as("task_under_test")

        @task = syskit_deploy(@task_m)
    end

    it "forwards a command sample" do
        command = Types.controldev.RawCommand.new(
            time: Time.at(1)
        )

        syskit_configure_and_start(task)

        expect_execution { syskit_write task.command_in_port, command }
            .to do
                have_one_new_sample(task.command_out_port)
                    .matching { |s| s.time == Time.at(1) }
            end
    end

    it "ignores commands with timestamp older than last fowarded command" do
        command = Types.controldev.RawCommand.new(
            time: Time.at(2)
        )

        syskit_configure_and_start(task)

        samples = expect_execution do
            syskit_write task.command_in_port, command
            syskit_write task.command_in_port, (command.tap { |c| c.time = Time.at(1) })
            syskit_write task.command_in_port, (command.tap { |c| c.time = Time.at(3) })
        end.to { have_new_samples(task.command_out_port, 2) } # rubocop:disable Style/MultilineBlockChain

        assert_equal Time.at(2), samples.first.time
        assert_equal Time.at(3), samples.last.time
    end
end
